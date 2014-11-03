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



struct SearchDataStruct
{
    CellType board[RowMax][ColumnMax];
    WhichPlayer whichPlayerToGo;
    int possibleDepth;
    int timeLimitationMS;
    int dtssDepth;
    int idDtssDepth;
    int movesToGo;
};

struct SearchResultStruct
{
    int r1, c1, r2, c2;
};

enum
{
    ParamSizeMax = 1024,
};

struct RequestDataStruct
{
    enum Type {
        TypeExit,
        TypeSearch,
        TypeSetParam
    } type;

    union Data {
        SearchDataStruct searchData;
        int param[ParamSizeMax];
    } data;
};

struct ResponseDataStruct {
    union Data {
        SearchResultStruct searchResult;
    } data;
};


struct SharedMemoryStruct
{
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
    , processStarted(false)
    , sharedMemory(sharedMemoryKey)
{

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

        if (req->type == RequestDataStruct::TypeExit) {

            Logger(logPath) << "exiting loop...\r\n";

            exitLoop = true;

        } else if (req->type == RequestDataStruct::TypeSearch) {

            searcher.SearchGoodMoves(req->data.searchData.board,
                                     req->data.searchData.whichPlayerToGo,
                                     req->data.searchData.movesToGo);

            Logger(logPath) << QString("got the data, begining search...\r\n");

            QThread::msleep(10 * 1000);

            memset(shared, 0, sizeof(SharedMemoryStruct));
            shared->requestProcessed = true;

            Logger(logPath) << QString("search finished, but the data is fake.\r\n");

        }

        sharedMemory.unlock();

        Logger(logPath) << "Shared Memory unlocked.\r\n";
    }

    sharedMemory.detach();

    Logger(logPath) << "Shared Memory detached.\r\n";
}

bool AIController::requestThinking(const BoardDataStruct &boardDataStruct)
{
    if (!processStarted) {
        start();
    }

    if (sharedMemory.isAttached()) {
        sharedMemory.detach();
    }

    if (!sharedMemory.create(sizeof(SharedMemoryStruct))) {
        Logger(logPath) << "cannot create Shared Memory." << "\r\n";
        return false;
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
    searchData->possibleDepth = 7;
    searchData->timeLimitationMS = 20000;
    searchData->dtssDepth = 9;
    searchData->idDtssDepth = 5;
    searchData->movesToGo = boardDataStruct.movesToGo;
    sharedMemoryStruct->requestProcessed = false;
    sharedMemoryStruct->data.request.type = RequestDataStruct::TypeSearch;

    sharedMemory.unlock();

    Logger(logPath) << "fillation finished, Shared Memory unlocked. key = " << sharedMemory.key() << "\r\n";

    return true;
}

void AIController::start()
{
    QStringList argv;
    argv.append("--ai");
    argv.append(sharedMemory.key());
    process.start(QCoreApplication::applicationFilePath(), argv);
    processStarted = true;
}
