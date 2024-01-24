#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QMediaCaptureSession *session; // 管理器
    QMediaRecorder *recorder; // 用于录音
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_stateChanged(QMediaRecorder::RecorderState state); // 状态变化
    void do_durationChanged(qint64 duration); // 录制时间变化
    void on_actRecord_triggered();

    void on_actPause_triggered();

    void on_btnGetFile_clicked();

    void on_actStop_triggered();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event); // 事件处理函数
};
#endif // MAINWINDOW_H
