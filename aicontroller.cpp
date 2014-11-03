#include "aicontroller.h"
#include <QSharedMemory>
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QCoreApplication>
#include <cassert>
#include <QTextStream>
#include "AI/defines.h"
#include "AI/MoveSearcher.h"
#include "logger.h"
#include <QDateTime>
#include <QMutexLocker>
#include "AI/boardformatter.h"


struct SearchDataStruct
{
    CellType board[RowMax][ColumnMax];
    WhichPlayer whichPlayerToGo;
    int alphaBetaDepth;
    int timeLimitationMS;
    int dtssDepth;
    int idDtssDepth;
    int movesToGo;
};

struct SearchResultStruct
{
    int r1, c1, r2, c2;
};

struct RequestDataStruct
{
    enum Type {
        TypeExit,
        TypeSearch,
    } type;

    union Data {
        SearchDataStruct searchData;
    } data;
};

struct ResponseDataStruct {
    union Data {
        SearchResultStruct searchResult;
    } data;
};


struct SharedMemoryStruct
{
    bool requestPending;
    bool requestProcessed;
    union Data {
        RequestDataStruct request;
        ResponseDataStruct response;
    } data;
};

AIController::AIController(const QString &sharedMemoryKey)
    : logPath(QCoreApplication::applicationFilePath() + "." +
              QString::number(QDateTime::currentMSecsSinceEpoch()) + "." +
              sharedMemoryKey + "." +
              QString::number(QCoreApplication::applicationPid()) + ".log")
    , threadLogPath(QCoreApplication::applicationFilePath() + "." +
                    QString::number(QDateTime::currentMSecsSinceEpoch()) + "." +
                    sharedMemoryKey + ".thread." +
                    QString::number(QCoreApplication::applicationPid()) + ".log")
    , processStarted(false)
    , sharedMemory(sharedMemoryKey)
    , waitingThreadStarted(false)
    , isResultReady(false)
    , resultR1(-1)
    , resultC1(-1)
    , resultR2(-1)
    , resultC2(-1)
{
}

AIController::~AIController()
{
    if (waitingThreadStarted) {
        waitingThread.quit();
        waitingThread.wait();
    }
}

void invokeSearcher(MoveSearcher *searcher, Board board, WhichPlayer whichPlayer, int movesToGo,
                    int &r1, int &c1, int &r2, int &c2)
{
    searcher->SearchGoodMoves(board,
                             whichPlayer,
                             movesToGo);

    r1 = searcher->GetDMove().GetPoint1()._row;
    c1 = searcher->GetDMove().GetPoint1()._col;
    r2 = searcher->GetDMove().GetPoint2()._row;
    c2 = searcher->GetDMove().GetPoint2()._col;
}

int AIController::exec()
{

    while (!sharedMemory.attach()) {
        Logger(logPath) << "failed to attach Shared Memory, key = "
                        << sharedMemory.key() << ", error = "
                        << sharedMemory.errorString() << "\r\n";
        QThread::msleep(500);
    }

    bool exitLoop = false;

    while(!exitLoop) {

        Logger(logPath) << "getting Shared Memory Lock...\r\n";

        sharedMemory.lock();

        Logger(logPath) << "got the lock.\r\n";

        SharedMemoryStruct *shared = (SharedMemoryStruct *)sharedMemory.constData();
        RequestDataStruct *req = (RequestDataStruct *)&shared->data.request;

        if (!shared->requestPending) {

            QThread::msleep(500);

        } else {

            if (req->type == RequestDataStruct::TypeExit) {

                Logger(logPath) << "exiting loop...\r\n";

                exitLoop = true;

            } else if (req->type == RequestDataStruct::TypeSearch) {

                Logger(logPath) << "got the data, begin search.\r\n";


                SearchResultStruct result;

                invokeSearcher(&gSearcher,
                               req->data.searchData.board,
                               req->data.searchData.whichPlayerToGo,
                               req->data.searchData.movesToGo,
                               result.r1,
                               result.c1,
                               result.r2,
                               result.c2);

                Logger(logPath) << "search finished\r\n";

                memset(shared, 0, sizeof(SharedMemoryStruct));

                shared->data.response.data.searchResult = result;

                shared->requestProcessed = true;
                shared->requestPending = false;

                Logger(logPath) << "search finished, (("
                                << (result.r1 + 1) << ", " << (char)('A' + result.c1) << "), ("
                                << (result.r2 + 1) << ", " << (char)('A' + result.c2) << ")).\r\n";

            }
        }

        sharedMemory.unlock();

        Logger(logPath) << "Shared Memory unlocked.\r\n";
    }

    sharedMemory.detach();

    Logger(logPath) << "Shared Memory detached.\r\n";
}

bool AIController::requestThinking(const BoardDataStruct &boardDataStruct, const AIParamStruct &param)
{
    if (!sharedMemory.isAttached()) {
        if (!sharedMemory.create(sizeof(SharedMemoryStruct))) {
            Logger(logPath) << "cannot create Shared Memory." << "\r\n";
            return false;
        }
    }

    sharedMemory.lock();

    Logger(logPath) << "Shared Memory created and locked, begin to fill these memory space.\r\n";

    SharedMemoryStruct *sharedMemoryStruct = (SharedMemoryStruct *)sharedMemory.data();
    SearchDataStruct *searchData = &sharedMemoryStruct->data.request.data.searchData;

    for (int i = 0; i < RowMax; ++i) {
        for (int k = 0; k < ColumnMax; ++k) {
            searchData->board[i][k] = boardDataStruct.board[i][k];
        }
    }
    searchData->whichPlayerToGo = boardDataStruct.whichPlayersTurn;
    searchData->movesToGo = boardDataStruct.movesToGo;

    searchData->alphaBetaDepth = param.alphaBetaDepth;
    searchData->timeLimitationMS = param.timeLimitationMS;
    searchData->dtssDepth = param.dtssDepth;
    searchData->idDtssDepth = param.idDtssDepth;

    sharedMemoryStruct->data.request.type = RequestDataStruct::TypeSearch;

    sharedMemoryStruct->requestProcessed = false;
    sharedMemoryStruct->requestPending = true;

    sharedMemory.unlock();

    Logger(logPath) << "fillation finished, Shared Memory unlocked. key = " << sharedMemory.key() << "\r\n";

    if (!processStarted) {
        startChildProcess();
    }

    return true;
}

bool AIController::getThinkingResult(int &r1, int &c1, int &r2, int &c2)
{
    if (!processStarted) {
        return false;
    }

    if (!waitingThreadStarted) {

        moveToThread(&waitingThread);

        connect(this, SIGNAL(readyToWait()), this, SLOT(doWait()));

        waitingThread.start();
        waitingThreadStarted = true;

        emit readyToWait();

        return false;

    } else {

        QMutexLocker ml(&resultMutex);

        if (isResultReady) {

            r1 = resultR1;
            c1 = resultC1;
            r2 = resultR2;
            c2 = resultC2;

            return true;

        } else {

            return false;
        }
    }
}


void AIController::doWait()
{
    Logger(threadLogPath) << "thread Do Wait\r\n";

    bool requestProcessed = false;

    while (!requestProcessed) {

        Logger(threadLogPath) << "getting Shared Memory Lock...\r\n";

        sharedMemory.lock();

        Logger(threadLogPath) << "got the lock.\r\n";

        SharedMemoryStruct *sharedMemoryStruct = (SharedMemoryStruct *)sharedMemory.data();

        if (!sharedMemoryStruct->requestProcessed) {

            Logger(threadLogPath) << "no request.\r\n";

        } else {

            SearchResultStruct *result = &sharedMemoryStruct->data.response.data.searchResult;

            sharedMemoryStruct->requestProcessed = false;

            {
                QMutexLocker ml(&resultMutex);

                resultR1 = result->r1;
                resultC1 = result->c1;
                resultR2 = result->r2;
                resultC2 = result->c2;
                isResultReady = true;
            }

            requestProcessed = true;

            Logger(threadLogPath) << "got result.\r\n";
        }

        sharedMemory.unlock();

        Logger(threadLogPath) << "Shared Memory unlocked.\r\n";

        if (!requestProcessed) {
            QThread::msleep(300);
        }
    }
}

void AIController::startChildProcess()
{
    QStringList argv;
    argv.append("--ai");
    argv.append(sharedMemory.key());
    process.start(QCoreApplication::applicationFilePath(), argv);
    processStarted = true;

    resultR1 = -1;
    resultC1 = -1;
    resultR2 = -1;
    resultC2 = -1;
    isResultReady = false;
}

void AIController::test()
{
    static char *testData = {
        "   A B C D E F G H I J K L M N o P Q R S" "\n"
        " 1 . . . . . . . . . . . . . . . . . . ." "\n"
        " 2 . . . . . . . . . . . . . . . . . . ." "\n"
        " 3 . . . . . . . . . . . . . . . . . . ." "\n"
        " 4 . . . . . . . . . . . . . . . . . . ." "\n"
        " 5 . . . . . . . . . . . . . . . . . . ." "\n"
        " 6 . . . . . . . . . . . . . . . . . . ." "\n"
        " 7 . . . . . X . O X . . . . . . . . . ." "\n"
        " 8 . . . . . . . . O X . . . . . . . . ." "\n"
        " 9 . . . . . . . O . X . . . . . . . . ." "\n"
        "10 . . . . . . . . O . . . . . . . . . ." "\n"
        "11 . . . . . . . . . . . . . . . . . . ." "\n"
        "12 . . . . . . . . . . . . . . . . . . ." "\n"
        "13 . . . . . . . . . . . X . . . . . . ." "\n"
        "14 . . . . . . . . . . . . . . . . . . ." "\n"
        "15 . . . . . . . . . . . . . . . . . . ." "\n"
        "16 . . . . . . . . . . . . . . . . . . ." "\n"
        "17 . . . . . . . . . . . . . . . . . . ." "\n"
        "18 . . . . . . . . . . . . . . . . . . ." "\n"
        "19 X . . . . . . . . . . . . . . . . . ." "\n"
    };

    int r1, c1, r2, c2;
    invokeSearcher(&gSearcher, BoardFormatter::stringToBoard(testData).cell,
                   WhitePlayer, 2, r1, c1, r2, c2);
}
