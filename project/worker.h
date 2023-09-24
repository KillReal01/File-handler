#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class MainWindow;

class Worker : public QObject
{
    Q_OBJECT

public:
    /*@brief Конструктор*/
    Worker(MainWindow *parent);

    /*@brief Деструктор*/
    ~Worker();

public slots:
     /*@brief Запуск отдельного потока, в котором выполняется обработка файлов.*/
    void doWork();

    /*@brief Локика обработки файлов.*/
    void handleFiles();

private:
    /*@\brief Указатель на главное окно.*/
    MainWindow *mw;

signals:
    /*@brief Сигнал завершения работы потока.*/
    void finished();

};

#endif // WORKER_H
