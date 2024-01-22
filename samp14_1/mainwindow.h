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
    TDiceThread *threadA; // 工作线程
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
    void do_threadA_newValue(int seq, int diceValue);
    void on_actThread_Run_triggered();
    void on_actThread_Quit_triggered();
    void on_actDice_Run_triggered();
    void on_actDice_Pause_triggered();
};
#endif // MAINWINDOW_H
