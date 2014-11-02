#include "aicontroller.h"
#include <QSharedMemory>
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QCoreApplication>
#include <cassert>



enum
{
    RowMax = 19,
    ColMax = 19,
    Infinity = 1000000000
};

enum CellType
{
    CellTypeBlack,
    CellTypeWhite,
    CellTypeEmpty
};

struct SearchDataStruct
{
    CellType board[RowMax][ColMax];
    bool isBlacksTurn;
    int countOfStones;
    int possibleDepth;
    int startTimeMS;
    int timeLimitationMS;
};

struct SearchResultStruct
{
    int _r1, _c1, _r2, _c2;
};

enum
{
    ParamSizeMax = 1024
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
        int param[PARAM_SIZE_MAX];
    } data;
};

struct ResponseDataStruct {
    union Data {
        SearchResultStruct searchResult;
    } data;
};


struct SharedMemoryStruct
{
};

AIController::AIController(): thread(NULL)
{

}


int AIController::exec(const QString &sharedMemoryKey)
{
    while(1) {}

    QSharedMemory sharedMemory(sharedMemoryKey);

    if (!sharedMemory.attach()) {
        return -1;
    }

    sharedMemory.lock();





    sharedMemory.unlock();

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
