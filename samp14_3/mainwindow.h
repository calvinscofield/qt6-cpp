#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tdicethread.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    TDiceThread *threadA; // producer
    TValueThread *threadValue; // consumer 1
    TPictureThread *threadPic; // consumer 2
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void do_threadA_started();
    void do_threadA_finished();
    void do_newValue(int seq, int diceValue); // 与threadValue 的 newValue()信号连接
    void do_newPicture(QString &picName); // 与threadPic 的newPicture()信号连接
    void on_actThread_Run_triggered();
    void on_actThread_Quit_triggered();
    void on_actClear_triggered();
};
#endif // MAINWINDOW_H
