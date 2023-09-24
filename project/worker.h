#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class MainWindow;

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(MainWindow *parent);
    ~Worker();

public slots:
    void doWork();
    void handleFiles();

private:
    MainWindow *mw;
signals:
    void finished();

};

#endif // WORKER_H
