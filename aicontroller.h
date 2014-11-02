#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "threadrunreimpelement.h"

class QThread;
class QString;

class AIController : public ThreadRunReimpelement::RunImpelementer
{
public:
    AIController();
    int exec(const QString &sharedMemoryKey);
    void start();
    void runImpl(QThread *);

private:
    QThread *thread;
};

#endif // AICONTROLLER_H
