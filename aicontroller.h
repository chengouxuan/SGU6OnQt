#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <QObject>
#include <QString>

#include "globaldef.h"
#include <QProcess>
#include <QSharedMemory>
#include <QThread>
#include <QMutex>


class AIController : public QObject
{
    Q_OBJECT

public:

    struct BoardDataStruct {
        CellType board[RowMax][ColumnMax];
        WhichPlayer whichPlayersTurn;
        int movesToGo;
    };

    struct AIParamStruct {
        int alphaBetaDepth;
        int timeLimitationMS;
        int dtssDepth;
        int idDtssDepth;
        AIParamStruct()
            : alphaBetaDepth(7)
            , timeLimitationMS(20000)
            , dtssDepth(9)
            , idDtssDepth(5) {}
        AIParamStruct(int argAlphaBetaDepth,
                      int argTimeLimit,
                      int argDtssDepth,
                      int argIdDtssDepth)
            : alphaBetaDepth(argAlphaBetaDepth)
            , timeLimitationMS(argTimeLimit)
            , dtssDepth(argDtssDepth)
            , idDtssDepth(argIdDtssDepth) {}
    };

public:

    AIController(const QString &sharedMemoryKey);
    ~AIController();
    int exec();
    bool requestThinking(const BoardDataStruct &boardDataStruct, const AIParamStruct &param = AIParamStruct());
    bool getThinkingResult(int &r1, int &c1, int &r2, int &c2);

private slots:
    void doWait();

signals:
    void readyToWait();
private:

    void startChildProcess();

private:

    QString logPath;
    QString threadLogPath;

    QProcess process;
    bool processStarted;

    QSharedMemory sharedMemory;

    QThread waitingThread;
    bool waitingThreadStarted;
    bool aWaitingIsDoing;

    bool isResultReady;

    int resultR1;
    int resultC1;
    int resultR2;
    int resultC2;

    QMutex resultMutex;


public:
    static void test();
};

#endif // AICONTROLLER_H
