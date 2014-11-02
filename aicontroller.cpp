#include "aicontroller.h"
#include <QSharedMemory>
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QCoreApplication>
#include <cassert>
#include <QFile>
#include "AI/defines.h"



struct SearchDataStruct
{
    CellType board[RowMax][ColumnMax];
    bool isBlacksTurn;
    int countOfStones;
    int possibleDepth;
    int startTimeMS;
    int timeLimitationMS;
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
    bool finishProcessing;
    union Data {
        RequestDataStruct request;
        ResponseDataStruct response;
    } data;
};

AIController::AIController(): thread(NULL)
{

}

void ReInitParams();

int AIController::exec(const QString &sharedMemoryKey)
{
    QSharedMemory sharedMemory(sharedMemoryKey);

    if (!sharedMemory.attach()) {
        return -1;
    }

    ReInitParams();

    while(1) {

        sharedMemory.lock();

        QFile log(QCoreApplication::applicationFilePath() + ".log");
        log.open(QFile::Append);

//        log << evaluator._evaluations << " evaluations\n";
//        log << evaluator._transTable._hits << " evaluator hits\n", evaluator._transTable._hits;
//        log << frame._nodes << " nodes\n";
//        log << (frame._transTableBlack._hits + frame._transTableWinte._hits) << " hits\n";
//        log << dtsser._nodes << " dtss nodes\n";
//        log << (dtsser._transTableBlack._hits + dtsser._transTableWhite._hits) << " dtss hits\n";
//        log << dtsser._dropedSearches << " dtss dropped searches\n";
//        lgo << dtsser._idtssSuccesses << " idtss successes\n";

        log.close();

        sharedMemory.unlock();
    }

    sharedMemory.detach();
}

void AIController::runImpl(QThread */*thread*/)
{
    QString sharedMemoryKey("uiopouy45698");
    QStringList argv;
    argv.append("--ai");
    argv.append(sharedMemoryKey);
    QProcess process;
    process.start(QCoreApplication::applicationFilePath(), argv);


    while(1) {}


    QSharedMemory sharedMemory(sharedMemoryKey);

    if (!sharedMemory.create(sizeof(SharedMemoryStruct))) {
        return;
    }

    sharedMemory.lock();

    char *to = (char*)sharedMemory.data();

    SharedMemoryStruct sharedMemoryStruct;



    const char *from = (const char *)&sharedMemoryStruct;
    assert(sharedMemory.size() == sizeof(SharedMemoryStruct));
    memcpy(to, from, sharedMemory.size());

    sharedMemory.unlock();
}

void AIController::start()
{
    delete thread;
    thread = new ThreadRunReimpelement(this);
    thread->start();
}
