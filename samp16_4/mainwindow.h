#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tmydevice.h"

#include <QMainWindow>
#include <QtCharts>
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
    const qint64 m_bufferSize = 10000; // 缓冲区大小，字节数
    bool m_isWorking = false; // 是否正在采集或播放
    QLineSeries *lineSeries; // 曲线序列
    QAudioSource *audioSource; // 用于采集原始音频
    TMyDevice *myDevice; // 用于显示的I/O设备

    QAudioSink *audioSink; // 用于播放原始音频
    QFile sinkFileDevice; // 用于audioSink播放音频时的I/O设备
    void iniChart(); // 初始化图表

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_IODevice_update(qint64 blockSize);
    void do_sink_stateChanged(QAudio::State state);
    void on_actPreferFormat_triggered();

    void on_actTest_triggered();

    void on_comboSampFormat_currentIndexChanged(int index);

    void on_spinChanCount_valueChanged(int arg1);

    void on_actStart_triggered();

    void on_actStop_triggered();

    void on_actPlayFile_triggered();

    void on_btnGetFile_clicked();

    void on_chkBoxSaveToFile_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event); // 事件处理函数
};
#endif // MAINWINDOW_H
