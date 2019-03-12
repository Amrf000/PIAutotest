#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H
#include <QTextEdit>
#include <QThread>
#include <QPushButton>
#include "emb.h"
#include "instconfig.h"

class MainWindow;
class WorkerThread : public QThread
{
public:
    explicit WorkerThread(MainWindow* pMain,QPushButton* pStart,InstConfig vec);
    virtual ~WorkerThread();
    void stop();
    void run();
public slots:
    void onReceive(QVariantList data);
private:
    MainWindow* m_pMain;
    QPushButton* m_pStart;
    InstConfig m_vec;
};
#endif // WORKERTHREAD_H
