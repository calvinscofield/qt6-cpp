#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tdicethread.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    TDiceThread *threadA; // 工作线程
    // calvin注释：原文上没说明要定义m_seq和m_diceValue
    int m_seq = 0, m_diceValue = 0;
    QTimer *timer; // 定时器
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
    void on_actThread_Run_triggered();
    void on_actThread_Quit_triggered();
    void on_actDice_Run_triggered();
    void on_actDice_Pause_triggered();
    void do_timeOut(); // 定时器的槽函数
};
#endif // MAINWINDOW_H
