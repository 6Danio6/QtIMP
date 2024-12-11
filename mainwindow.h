#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QImage* img;
    QImage* lastImg;
    QPixmap pixmap;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_actionDesaturation_triggered();
    void on_actionInvert_triggered();
    void on_actionContrast_triggered();
    void on_actionBrightness_triggered();

    void on_actionUndo_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
