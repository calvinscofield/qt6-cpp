#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
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
    QMediaPlayer *player; // 播放器
    bool loopPlay = true; // 是否循环播放
    QString durationTime; // 总时长，mm:ss字符串
    QString positionTime; // 当前播放到的位置，mm:ss字符串
    QUrl getUrlFromItem(QListWidgetItem *item); // 获取item的用户数据
    bool eventFilter(QObject *watched, QEvent *event); // 事件过滤处理
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void do_stateChanged(QMediaPlayer::PlaybackState state); // 播放器状态发生变化
    void do_sourcChanged(const QUrl &media); // 播放源发生变化
    void do_durationChanged(qint64 duration); // 播放时长发生变化
    void do_positionChanged(qint64 position); // 播放位置发生变化
    void do_metaDataChanged(); // 元数据发生变化
    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnClear_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_btnPrevious_clicked();

    void on_btnPlay_clicked();

    void on_btnPause_clicked();

    void on_btnStop_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);


    void on_btnLoop_clicked(bool checked);

    void on_sliderPosition_valueChanged(int value);

    void on_btnSound_clicked();

    void on_sliderVolumn_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
