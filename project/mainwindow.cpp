#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QFileDialog>
#include <QRegularExpression>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Модификация файлов");

    // radioButtons
    buttonGroupDelete.addButton(ui->radioButton_delete_yes);
    buttonGroupDelete.addButton(ui->radioButton_delete_no);
    ui->radioButton_delete_no->setChecked(true);

    buttonGroupRepeat.addButton(ui->radioButton_repeat_rewrite);
    buttonGroupRepeat.addButton(ui->radioButton_repeat_modify);
    ui->radioButton_repeat_modify->setChecked(true);

    buttonGroupTime.addButton(ui->radioButton_timer);
    buttonGroupTime.addButton(ui->radioButton_one_time_run);
    ui->radioButton_one_time_run->setChecked(true);

    // doubleSpinBox
    ui->doubleSpinBox_timer->hide();
    ui->doubleSpinBox_timer->setSingleStep(0.1);

    // lineEdit for mask
    QRegularExpression regex("[0-9A-Fa-f]{16}");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->lineEdit_modify_key->setValidator(validator);

    ui->lineEdit_modify_key->setMaxLength(MainWindow::size * 2);
    ui->lineEdit_modify_key->setInputMask("HHHHHHHHHHHHHHHH");


    // thread
    worker = new Worker(this);
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::started, worker, [this]() {
        ui->pushButton_run->setEnabled(false);
        worker->doWork();
    });

    connect(worker, &Worker::finished, this, [this]() {
        workerThread.quit();
        workerThread.wait();
        ui->pushButton_run->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete worker;
}


namespace{
    std::vector<int> convertToInt(QString& input)
    {
        std::vector<int> result(MainWindow::size);
        for(int i = 0; i < MainWindow::size; ++i){
            QString hexString = QString(input[2 * i]) + QString(input[2 * i + 1]);
            bool ok;
            int decimalValue = hexString.toInt(&ok, MainWindow::size * 2);
            if (ok){
                result[i] = decimalValue;
            }
        }
        return result;
    }

    void modifyOperation(char* data, std::vector<int>& vec)
    {
        for (int i = 0; i < MainWindow::size; ++i){
            data[i] ^= vec[i];
        }
    }
}

void MainWindow::init_values()
{
    file_mask = "*" + ui->lineEdit_mask->text() + "*";
    time = ui->doubleSpinBox_timer->value();
    key = ui->lineEdit_modify_key->text();

    file_remove = ui->radioButton_delete_yes->isChecked();
    file_rewrite = ui->radioButton_repeat_rewrite->isChecked();
    file_timer = ui->radioButton_timer->isChecked();

    inputDirectory.setPath(ui->lineEdit_init_directory->text());
    outputDirectory.setPath(ui->lineEdit_result_path->text());
}

void MainWindow::handleFile(QFile& inputFile, QFile& outputFile)
{
    QDataStream in(&inputFile);
    QDataStream out(&outputFile);

    while (!in.atEnd()) {
        char buffer[MainWindow::size];
        qint64 bytesRead = in.readRawData(buffer, MainWindow::size);
        modifyOperation(buffer, encryption);
        out.writeRawData(buffer, bytesRead);
    }
}


void MainWindow::modifyFilename(QFile& outputFile, QString& outputFilePath)
{
    int i = 1;
    if (!file_rewrite){
        while(outputFile.exists()){
            QFileInfo fileInfo(outputFilePath);
            QString baseName = fileInfo.baseName();
            QString suffix = fileInfo.suffix();
            outputFile.setFileName(outputDirectory.filePath(baseName + "_" + QString::number(i) + "." + suffix));
            i++;
        }
    }
}

void MainWindow::on_pushButton_run_clicked()
{
    init_values();

    // get encryption key
    encryption = convertToInt(key);
    workerThread.start();
}


void MainWindow::on_pushButton_search_clicked()
{
    QString initialDir = QCoreApplication::applicationDirPath();
    QString selectedDir = QFileDialog::getExistingDirectory(this, "Выберите каталог", initialDir);
    ui->lineEdit_result_path->setText(selectedDir);
}


void MainWindow::on_pushButton_init_directory_clicked()
{
    QString initialDir = QCoreApplication::applicationDirPath();
    QString selectedDir = QFileDialog::getExistingDirectory(this, "Выберите каталог", initialDir);
    ui->lineEdit_init_directory->setText(selectedDir);
}


void MainWindow::on_pushButton_stop_clicked()
{
    run = false;
}


void MainWindow::on_radioButton_timer_clicked()
{
    ui->doubleSpinBox_timer->show();
}


void MainWindow::on_radioButton_one_time_run_clicked()
{
    ui->doubleSpinBox_timer->hide();
}

