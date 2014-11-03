#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <QObject>
#include <QString>

#include "globaldef.h"
#include <QProcess>
#include <QSharedMemory>


class AIController
{

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
    int exec();
    bool requestThinking(const BoardDataStruct &boardDataStruct, const AIParamStruct &param = AIParamStruct());

private:

    void start();

private:

    QString logPath;

    QProcess process;
    bool processStarted;

    QSharedMemory sharedMemory;

};

#endif // AICONTROLLER_H
