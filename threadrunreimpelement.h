#ifndef AICONTROLTHREAD_H
#define AICONTROLTHREAD_H

#include <QThread>

class ThreadRunReimpelement : public QThread
{
    Q_OBJECT

public:

    class RunImpelementer {
    public:
        void virtual runImpl(QThread *) = 0;
    };

private:
    RunImpelementer *runImpelementer;

    void run() Q_DECL_OVERRIDE {
        if (runImpelementer != NULL) {
            runImpelementer->runImpl(this);
        }
    }

public:
    explicit ThreadRunReimpelement(RunImpelementer *runImp = NULL, QObject *parent = 0)
        : runImpelementer(runImp) {}

signals:

public slots:

};

#endif // AICONTROLTHREAD_H
