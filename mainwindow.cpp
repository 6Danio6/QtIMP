#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "imageutil.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>
#include <QSlider>

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // QLabel *label = new QLabel("Centered Widget");
    // label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(ui->centralWidget);
    layout->addWidget(ui->imageDisplay, 0, Qt::AlignCenter);
}

// File section
void MainWindow::on_actionOpen_triggered()
{
    auto fileContentReady = [&](const QString &fileName, const QByteArray &fileContent) {
        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "No File Selected", "No file was selected.");
            qDebug() << "No file was selected.";
            return 1;
        }
        qDebug() << "fileName: " << fileName;

        if (fileContent[0] != 'P')
        {
            QMessageBox::critical(this, "Invalid File", "Invalid file format. File should start with P1-P6.");
            return 1;
        }

        switch (fileContent[1])
        {
        case '1':
        {
            img = ImageUtil::parseP1(fileContent);
            break;
        }
        case '2':
        {
            img = ImageUtil::parseP2(fileContent);
            break;
        }
        case '3':
        {
            img = ImageUtil::parseP3(fileContent);
            break;
        }
        default:
            QMessageBox::critical(this, "Invalid File", "Invalid file format. File should start with P1-P6.");
            return 1;
        }

        pixmap = QPixmap::fromImage(*img);
        ui->imageDisplay->setPixmap(pixmap);

        // ImageUtil::printToDebug(*img);
        return 0;
    };

    QFileDialog::getOpenFileContent("Portable Anymap Files (*.pbm *.pgm *.ppm *.pnm);;All Files (*)",  fileContentReady);
}

void MainWindow::on_actionSave_triggered()
{
    QStringList items;
    items << "P1" << "P2" << "P3" << "P4" << "P5" << "P6";

    bool ok;
    QString format = QInputDialog::getItem(this, "Choose output format", "Anymap Format:", items, 0, false, &ok);
    if (!ok || format.isEmpty())
    {
        QMessageBox::critical(this, "Invalid option", "Invalid option.");
        return;
    }

    QString fileName = QInputDialog::getText(this, "Choose file name", "File name:",
                                             QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (!ok || fileName.isEmpty())
    {
        QMessageBox::critical(this, "Invalid option", "Invalid option.");
        return;
    }

    switch (format[1].digitValue())
    {
    case 1:
    {
        ImageUtil::saveP1(*img, fileName);
        break;
    }
    case 2:
    {
        ImageUtil::saveP2(*img, fileName);
        break;
    }
    case 3:
    {
        ImageUtil::saveP3(*img, fileName);
        break;
    }
    default:
        QMessageBox::critical(this, "Invalid option", "Invalid option.");
        return;
    }
}

// Filter section
void MainWindow::on_actionDesaturation_triggered()
{
    lastImg = img;
    img = ImageUtil::desaturate(img);
    pixmap = QPixmap::fromImage(*img);
    ui->imageDisplay->setPixmap(pixmap);
    ui->actionUndo->setEnabled(true);
}

void MainWindow::on_actionInvert_triggered()
{
    lastImg = img;
    img = ImageUtil::invert(img);
    pixmap = QPixmap::fromImage(*img);
    ui->imageDisplay->setPixmap(pixmap);
    ui->actionUndo->setEnabled(true);
}

void MainWindow::on_actionContrast_triggered()
{
    lastImg = img;
    img = ImageUtil::contrast(img);
    pixmap = QPixmap::fromImage(*img);
    ui->imageDisplay->setPixmap(pixmap);
    ui->actionUndo->setEnabled(true);
}

void MainWindow::on_actionBrightness_triggered()
{
    QMessageBox *dialog = new QMessageBox(ui->centralWidget);
    // dialog->setModal(true);
    dialog->setWindowTitle("Change brightness");
    dialog->setText("Change brightness");
    dialog->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    dialog->setDefaultButton(QMessageBox::Save);
    dialog->setContentsMargins(50, 50, 50, 50);
    QSlider *slider = new QSlider(Qt::Horizontal, dialog);
    slider->setRange(-10, 10);
    slider->setValue(0);
    slider->setContentsMargins(20, 20, 20, 20);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setTickInterval(10);

    int returned = dialog->exec();
    switch (returned)
    {
    case QMessageBox::Save:
    {
        // Save was clicked
        double sliderValue = -(double)slider->value() / 10;
        lastImg = img;
        img = ImageUtil::brightness(img, sliderValue);
        pixmap = QPixmap::fromImage(*img);
        ui->imageDisplay->setPixmap(pixmap);
        ui->actionUndo->setEnabled(true);

        break;
    }
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

// Edit section
void MainWindow::on_actionUndo_triggered()
{
    img = lastImg;
    lastImg = nullptr;
    pixmap = QPixmap::fromImage(*img);
    ui->imageDisplay->setPixmap(pixmap);
    ui->actionUndo->setEnabled(false);
}
