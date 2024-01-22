#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mythread.h"

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
    TDaqThread *threadDAQ; // 数据采集线程
    TProcessThread *threadShow; // 数据处理线程
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void do_readBuffer(int bufferSeq, int *bufferData, int pointCount);
    void do_threadA_started();
    void do_threadA_finished();
    void on_actThread_Run_triggered();
    void on_actThread_Quit_triggered();
    void on_actClear_triggered();
};
#endif // MAINWINDOW_H
