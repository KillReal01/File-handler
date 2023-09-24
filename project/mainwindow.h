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
    /*@brief Конструктор*/
    MainWindow(QWidget *parent = nullptr);

    /*@brief Деструктор*/
    ~MainWindow();

    /*@brief Количество байт маски*/
    static const int size = 8;

private:
    /*@brief Дружественный класс Worker*/
    friend class Worker;

    /*@brief Указатель на форму*/
    Ui::MainWindow *ui;

    /*@brief Указатель на Worker*/
    Worker* worker;

    /*@brief Поток, обрабатывающий файлы*/
    QThread workerThread;

    /*@brief Флаг обработки файлов*/
    std::atomic<bool> run;

    /*@brief Вектор с байтами маски*/
    std::vector<uint8_t> encryption;

    /*@brief Блок удаления файлов */
    QButtonGroup buttonGroupDelete;

    /*@brief Блок повторения файлов */
    QButtonGroup buttonGroupRepeat;

    /*@brief Блок таймера */
    QButtonGroup buttonGroupTime;

    /*@brief Маска файлов */
    QString file_mask;

    /*@brief Время опроса */
    double time;

    /*@brief Ключ шифрования */
    QString key;

    /*@brief Удаление файла */
    bool file_remove;

    /*@brief Перезапись файла */
    bool file_rewrite;

    /*@brief Наличие таймера */
    bool file_timer;

    /*@brief Входная директория */
    QDir inputDirectory;

    /*@brief Выходная директория */
    QDir outputDirectory;

    /*@brief Чтение данных с формы */
    void init_values();

    /*@brief Логика обработки файла */
    void handleFile(QFile& inputFile, QFile& outputFile);

    /*@brief Модификация имени файла */
    void modifyFilename(QFile&, QString&);

private slots:
    /*@brief Слот для запуска программы */
    void on_pushButton_run_clicked();

    /*@brief Слот для обзора директории для сохранения */
    void on_pushButton_search_clicked();

    /*@brief Слот для обзора входной директории */
    void on_pushButton_init_directory_clicked();

    /*@brief Слот для остановки программы */
    void on_pushButton_stop_clicked();

    /*@brief Слот обработки radioButton timer */
    void on_radioButton_timer_clicked();

    /*@brief Слот обработки radioButton one time run */
    void on_radioButton_one_time_run_clicked();
};
#endif // MAINWINDOW_H
