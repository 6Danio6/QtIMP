#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "image.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    // QString fileName = QFileDialog::getOpenfileName(this, "Open File", "", "Text Files (*.txt);;All Files (*)");
    // QString fileName = QFileDialog::getOpenfileName(this, "Select an image file", "",
    //                                                 "Portable Anymap Files (*.pbm *.pgm *.ppm *.pnm);;All Files (*)");
    // if (!fileName.isEmpty()) {
    //     QMessageBox::information(this, "Selected File", fileName);
    // }

    // QByteArray fileContent;
    // QFileDialog::getOpenFileContent("Text Files (*.txt);;All Files (*)", &fileContent);

    // if (fileContent.isEmpty()) {
    //     QMessageBox::warning(this, "No File Selected", "No file was selected.");
    //     return;
    // }

    // // Convert file content to a string
    // QString content = QString::fromUtf8(fileContent);
    // QMessageBox::information(this, "File Opened", "File path: " + "nuh uh" + "\nContent:\n" + content);

    auto fileContentReady = [&](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "No File Selected", "No file was selected.");
            qDebug() << "No file was selected.";
            return 1;
        }
        qDebug() << "fileName: " << fileName;
        qDebug() << "fileContent: " << fileContent;

        if (fileContent[0] != 'P')
        {
            QMessageBox::warning(this, "Invalid File", "Invalid file format. File should start with P1-P6.");
            return 1;
        }

        switch (fileContent[1])
        {
        case '1':
        {
            Image img;
            img.parseP1(fileContent);
            img.printImage();
            break;
        }
        default:
            QMessageBox::warning(this, "Invalid File", "Invalid file format. File should start with P1-P6.");
            return 1;
        }

        // if (fileContent.contains('\n')) // ASCII/"plain" encoding
        // {
        //     qDebug() << fileContent[0];
        //     QList<QByteArray> BAList = fileContent.split('\n');

        //     for (auto i = BAList.cbegin(), end = BAList.cend(); i != end; ++i)
        //         qDebug() << *i;
        // }
        // else // binary/"raw" encoding
        // {

        // }
        return 0;
    };
    QFileDialog::getOpenFileContent("Portable Anymap Files (*.pbm *.pgm *.ppm *.pnm);;All Files (*)",  fileContentReady);
    qDebug() << "xd";
}

