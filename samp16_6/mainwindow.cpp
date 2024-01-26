#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this); // 创建播放器
    QAudioOutput *audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput); // 设置音频输出通道

    QGraphicsScene *scene = new QGraphicsScene(this); // 创建场景
    ui->graphicsView->setScene(scene); // 为试图设置场景
    videoItem = new QGraphicsVideoItem; // 常见视频显示图形项
    videoItem->setSize(QSizeF(360, 240));
    videoItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    scene->addItem(videoItem);
    player->setVideoOutput(videoItem); // 设置视频显示图形项
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::do_stateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::do_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged);

    QGraphicsSimpleTextItem *item2 = new QGraphicsSimpleTextItem("海风吹，海浪涌");
    QFont font = item2->font();
    font.setPointSize(20);
    item2->setFont(font);
    item2->setPos(0, 0);
    item2->setBrush(QBrush(Qt::blue));
    item2->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    scene->addItem(item2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{
    // 播放器状态变化
    bool isPlaying = (state == QMediaPlayer::PlayingState);
    ui->btnPlay->setEnabled(!isPlaying);
    ui->btnPause->setEnabled(isPlaying);
    ui->btnStop->setEnabled(isPlaying);
}

void MainWindow::do_durationChanged(qint64 duration)
{
    // 文件时长变化
    ui->sliderPosition->setMaximum(duration);
    int secs = duration / 1000; // 秒
    int mins = secs / 60; // 分钟
    secs = secs % 60; // 取余数秒
    durationTime = QString::asprintf("%d:%d", mins, secs);
    ui->LabRatio->setText(positionTime + "/" + durationTime);
}

void MainWindow::do_positionChanged(qint64 position)
{
    // 文件播放位置变化
    if (ui->sliderPosition->isSliderDown())
        return; // 如果正在拖动滑动条，退出
    ui->sliderPosition->setSliderPosition(position);
    int secs = position / 1000; // 秒
    int mins = secs / 60; // 分钟
    secs = secs % 60; // 取余数秒
    positionTime = QString::asprintf("%d:%d", mins, secs);
    ui->LabRatio->setText(positionTime + "/" + durationTime);
}

void MainWindow::on_btnAdd_clicked()
{
    // 打开文件
    QString curPath = QDir::homePath();
    QString dlgTitle = "选择视频文件";
    QString filter = "视频文件(*.wmv, *.mp4);;所有文件(*.*)";
    QString aFile = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
    if (aFile.isEmpty())
        return;
    QFileInfo fileInfo(aFile);
    ui->LabCurMedia->setText(fileInfo.fileName());
    player->setSource(QUrl::fromLocalFile(aFile)); // 设置播放文件
    player->play();
}


void MainWindow::on_btnPlay_clicked()
{
    // 播放
    player->play();
}


void MainWindow::on_btnPause_clicked()
{
    // 暂停
    player->pause();
}


void MainWindow::on_btnStop_clicked()
{
    // 停止
    player->stop();
}


void MainWindow::on_sliderVolumn_valueChanged(int value)
{
    // 调节音量
    player->audioOutput()->setVolume(value / 100.0);
}


void MainWindow::on_btnSound_clicked()
{
    // 静音按钮
    bool mute = player->audioOutput()->isMuted();
    player->audioOutput()->setMuted(!mute);
    if (mute)
        ui->btnSound->setIcon(QIcon(":/images/images/volumn.bmp"));
    else
        ui->btnSound->setIcon(QIcon(":/images/images/mute.bmp"));
}


void MainWindow::on_sliderPosition_valueChanged(int value)
{
    // 播放位置
    player->setPosition(value);
}

void MainWindow::on_btnZoomIn_clicked()
{
    // 放大
    qreal factor = videoItem->scale();
    videoItem->setScale(factor + 0.1);
}


void MainWindow::on_btnZoomOut_clicked()
{
    // 缩小
    qreal factor = videoItem->scale();
    if (factor >= 0.2)
        videoItem->setScale(factor - 0.1);
}

