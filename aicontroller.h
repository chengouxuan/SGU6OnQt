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

    };

public:

    AIController(const QString &sharedMemoryKey);
    int exec();
    bool requestThinking(const BoardDataStruct &boardDataStruct);

private:

    void start();

private:

    QString logPath;

    QProcess process;
    bool processStarted;

    QSharedMemory sharedMemory;

};

#endif // AICONTROLLER_H
