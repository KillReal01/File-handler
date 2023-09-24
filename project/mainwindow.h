#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "worker.h"
#include <QMainWindow>
#include <QString>
#include <QButtonGroup>
#include <QDir>
#include <QTimer>
#include <QThread>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static const int size = 8;

private:
    friend class Worker;
    Ui::MainWindow *ui;

    Worker* worker;
    QThread workerThread;

    std::atomic<bool> run;
    std::vector<int> encryption;

    QButtonGroup buttonGroupDelete;
    QButtonGroup buttonGroupRepeat;
    QButtonGroup buttonGroupTime;

    QString file_mask;
    double time;
    QString key;

    bool file_remove;
    bool file_rewrite;
    bool file_timer;

    QDir inputDirectory;
    QDir outputDirectory;

    void handleFile(QFile& inputFile, QFile& outputFile);
    void modifyFilename(QFile&, QString&);

private slots:
    void init_values();
    void on_pushButton_run_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_init_directory_clicked();
    void on_pushButton_stop_clicked();
    void on_radioButton_timer_clicked();
    void on_radioButton_one_time_run_clicked();
};
#endif // MAINWINDOW_H
