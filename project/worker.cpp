#include "worker.h"
#include "mainwindow.h"
#include <QDebug>
#include <QFile>


Worker::Worker(MainWindow *parent)
{
    mw = parent;
}


Worker::~Worker()
{
    delete mw;
}


void Worker::doWork()
{
    handleFiles();
    if (mw->file_timer){
        mw->run = true;
        while(mw->run){
            handleFiles();
        }
    }
    emit finished();
}


void Worker::handleFiles()
{
    QStringList fileList = mw->inputDirectory.entryList(QStringList(mw->file_mask), QDir::Files);

    foreach (QString fileName, fileList) {
        QString inputFilePath =  mw->inputDirectory.filePath(fileName);
        QString outputFilePath =  mw->outputDirectory.filePath("result_" + fileName);

        QFile inputFile(inputFilePath);
        QFile outputFile(outputFilePath);

        mw->modifyFilename(outputFile, outputFilePath);

        if (!inputFile.isOpen() && inputFile.open(QIODevice::ReadOnly))
        {
            if (!outputFile.isOpen() && outputFile.open(QIODevice::WriteOnly))
            {
                mw->handleFile(inputFile, outputFile);
                inputFile.close();
                outputFile.close();

                if (mw->file_remove)
                    inputFile.remove();

                qDebug() << "Файл успешно создан и записан:" << outputFile.fileName();

            } else {
                qDebug() << "Не удалось открыть файл " << outputFile.fileName();
            }
        } else {
            qDebug() << "Не удалось открыть файл " << inputFile.fileName();
        }
    }
    if (mw->file_timer)
        QThread::msleep(1000 * mw->time);
}
