#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

QUrl MainWindow::getUrlFromItem(QListWidgetItem *item)
{
    // 返回item的用户数据
    QVariant itemData = item->data(Qt::UserRole); // 获取用户数据
    QUrl source = itemData.value<QUrl>(); // QVariant转换为QUrl类型
    return source;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() != QEvent::KeyPress) // 不是KeyPress事件，退出
        return QWidget::eventFilter(watched, event);
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() != Qt::Key_Delete) // 按下的不是Delete键，退出
        return QWidget::eventFilter(watched, event);
    if (watched == ui->listWidget)
    {
        QListWidgetItem *item = ui->listWidget->takeItem(ui->listWidget->currentRow());
        delete item;
    }
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->installEventFilter(this); // 安装事件过滤器
    ui->listWidget->setDragEnabled(true); // 允许拖放操作
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove); // 列表项可在组件内部被拖放
    player = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this); // 音频输出，指向默认的音频输出设备
    player->setAudioOutput(audioOutput); // 设置音频输出
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::do_positionChanged); // 播放位置发生变化
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged); // 播放时长发生变化
    connect(player, &QMediaPlayer::sourceChanged, this, &MainWindow::do_sourcChanged); // 播放源发生变化
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::do_stateChanged); // 播放器状态发生变化
    connect(player, &QMediaPlayer::metaDataChanged, this, &MainWindow::do_metaDataChanged); // 元数据发生变化
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_stateChanged(QMediaPlayer::PlaybackState state)
{
    // 播放器状态变化时运行，更新按钮状态，或播放下一曲
    ui->btnPlay->setEnabled(state != QMediaPlayer::PlayingState);
    ui->btnPause->setEnabled(state == QMediaPlayer::PlayingState);
    ui->btnStop->setEnabled(state == QMediaPlayer::PlayingState);
    // 播放完一曲后停止，如果loopPlay为true，自动播放下一曲
    if (loopPlay && (state == QMediaPlayer::StoppedState))
    {
        int count = ui->listWidget->count();
        int curRow = ui->listWidget->currentRow();
        curRow++;
        curRow = curRow >= count ? 0 : curRow;
        ui->listWidget->setCurrentRow(curRow);
        player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
        player->play();
    }
}

void MainWindow::do_sourcChanged(const QUrl &media)
{
    // 播放的文件发生变化时运行
    ui->labCurMedia->setText(media.fileName()); // 显示当前播放的曲目名称，不带路径
}

void MainWindow::do_durationChanged(qint64 duration)
{
    // 播放源时长发生变化时运行，更新进度显示
    ui->sliderPosition->setMaximum(duration);
    int secs = duration / 1000; // 秒
    int mins = secs / 60; // 分钟
    secs = secs % 60; // 取余数秒
    durationTime = QString::asprintf("%d:%d", mins, secs);
    ui->labRatio->setText(positionTime + "/" + durationTime);
}

void MainWindow::do_positionChanged(qint64 position)
{
    // 播放位置发生变化时运行，更新进度显示
    if (ui->sliderPosition->isSliderDown()) // 滑条正在被鼠标拖动
        return;
    ui->sliderPosition->setSliderPosition(position);
    int secs = position / 1000; // 秒
    int mins = secs / 60; // 分钟
    secs = secs % 60; // 取余数秒
    positionTime = QString::asprintf("%d:%d", mins, secs);
    ui->labRatio->setText(positionTime + "/" + durationTime);
}

void MainWindow::do_metaDataChanged()
{
    // 元数据变化时运行，显示歌曲对应的图片
    QMediaMetaData metaData = player->metaData(); // 元数据对象
    QVariant metaImg = metaData.value(QMediaMetaData::ThumbnailImage); // 图片数据
    if (metaImg.isValid())
    {
        QImage img = metaImg.value<QImage>(); // QVariant转换为QImage
        QPixmap musicPixmp = QPixmap::fromImage(img);
        if (ui->scrollArea->width() < musicPixmp.width())
            ui->labPic->setPixmap(musicPixmp.scaledToWidth(ui->scrollArea->width() - 30));
        else
            ui->labPic->setPixmap(musicPixmp);
    }
    else
        ui->labPic->clear();
}

void MainWindow::on_btnAdd_clicked()
{
    // “添加”按钮，添加文件
    QString curPath = QDir::homePath(); // 获取系统当前目录
    QString dlgTitle = "选择音频文件";
    QString filter = "音频文件(*.mp3 *.wav *.wma);;所有文件(*.*)"; // 文件过滤器
    QStringList fileList = QFileDialog::getOpenFileNames(this, dlgTitle, curPath, filter);
    if (fileList.count() < 1)
        return;
    for (int i = 0; i < fileList.size(); i++)
    {
        QString aFile = fileList.at(i);
        QFileInfo fileInfo(aFile);
        QListWidgetItem *aItem = new QListWidgetItem(fileInfo.fileName());
        aItem->setIcon(QIcon(":/images/images/musicFile.png"));
        aItem->setData(Qt::UserRole, QUrl::fromLocalFile(aFile)); // 设置用户数据
        ui->listWidget->addItem(aItem); // 添加到界面上的列表
    }

    if (player->playbackState() != QMediaPlayer::PlayingState)
    {
        // 当前没有播放则播放第一个文件
        ui->listWidget->setCurrentRow(0);
        QUrl source = getUrlFromItem(ui->listWidget->currentItem()); // 获取播放媒介
        player->setSource(source); // 设置播放媒介
    }
    player->play();
}


void MainWindow::on_btnRemove_clicked()
{
    // “移除”按钮，移除列表中的当前项
    int index = ui->listWidget->currentRow();
    if (index >= 0)
    {
        QListWidgetItem *item = ui->listWidget->takeItem(index);
        delete item;
    }
}


void MainWindow::on_btnClear_clicked()
{
    // “清空”按钮，清空播放列表
    loopPlay = false; // 防止do_stateChanged()里切换曲目
    ui->listWidget->clear();
    player->stop();
}


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    // 双击listWidget时切换曲目
    Q_UNUSED(index);
    loopPlay = false; // 暂时设置为false，防止do_stateChanged()切换曲目
    player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    player->play();
    loopPlay = ui->btnLoop->isChecked();
}


void MainWindow::on_btnPrevious_clicked()
{
    // 前一曲
    int curRow = ui->listWidget->currentRow();
    curRow--;
    curRow = curRow < 0 ? 0 : curRow;
    ui->listWidget->setCurrentRow(curRow); // 设置当前行
    loopPlay = false; // 暂时设置为false，防止do_stateChanged()切换曲目
    player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    player->play();
    loopPlay = ui->btnLoop->isChecked();
}


void MainWindow::on_btnPlay_clicked()
{
    // 开始播放
    if (ui->listWidget->currentRow() < 0) // 没有选择曲目则播放第一个
        ui->listWidget->setCurrentRow(0);
    player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    loopPlay = ui->btnLoop->isChecked(); // 是否循环播放
    player->play();
}


void MainWindow::on_btnPause_clicked()
{
    // 暂停播放
    player->pause();
}


void MainWindow::on_btnStop_clicked()
{
    // 停止播放
    loopPlay = false;
    player->stop();
}


void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    // “倍速”
    player->setPlaybackRate(arg1);
}



void MainWindow::on_btnLoop_clicked(bool checked)
{
    // “循环”播放
    loopPlay = checked;
}


void MainWindow::on_sliderPosition_valueChanged(int value)
{
    // 播放进度控制
    player->setPosition(value);
}


void MainWindow::on_btnSound_clicked()
{
    // 静音控制
    bool mute = player->audioOutput()->isMuted();
    player->audioOutput()->setMuted(!mute);
    if (mute)
        ui->btnSound->setIcon(QIcon(":/images/images/volumn.bmp"));
    else
        ui->btnSound->setIcon(QIcon(":/images/images/mute.bmp"));
}


void MainWindow::on_sliderVolumn_valueChanged(int value)
{
    // 调整音量
    player->audioOutput()->setVolume(value / 100.0); // 0~1
}

