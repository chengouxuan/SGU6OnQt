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

    class BoardData {

    public:

        virtual CellType cellTypeAt(int row, int column) = 0;
        virtual WhichPlayer whichPlayersTurn() = 0;
        virtual int movesToGo() = 0;
    };

public:

    AIController(const QString &sharedMemoryKey);
    int exec();
    bool requestThinking(BoardData *boardData);

private:

    void start();

private:

    QString logPath;

    QProcess process;
    bool processStarted;

    QSharedMemory sharedMemory;

};

#endif // AICONTROLLER_H
