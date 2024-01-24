#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    session = new QMediaCaptureSession(this); // 管理器
    QAudioInput *audioInput = new QAudioInput(this);
    session->setAudioInput(audioInput); // 为session设置音频输入设备
    recorder = new QMediaRecorder(this);
    session->setRecorder(recorder); // 为session设置recorder
    connect(recorder, &QMediaRecorder::recorderStateChanged, this, &MainWindow::do_stateChanged);
    connect(recorder, &QMediaRecorder::durationChanged, this, &MainWindow::do_durationChanged);

    if (QMediaDevices::defaultAudioInput().isNull()) // 如果没有默认的音频输入设备
    {
        ui->groupBoxDevice->setTitle("录制设置（无设备）");
        ui->actRecord->setEnabled(false);
        QMessageBox::information(this, "提示", "无音频输入设备");
        return; // 无音频输入设备，下面的数据就都没有
    }
    foreach (QAudioDevice device, QMediaDevices::audioInputs()) // 音频输入设备列表
        ui->comboDevices->addItem(device.description(), QVariant::fromValue(device));

    QMediaFormat format; // 默认的格式对象
    foreach (QMediaFormat::AudioCodec encoder, format.supportedAudioCodecs(QMediaFormat::Encode)) // 支持的编码格式
        ui->comboCodec->addItem(QMediaFormat::audioCodecDescription(encoder), QVariant::fromValue(encoder)); // 添加用户数据
    foreach (QMediaFormat::FileFormat fileFormat, format.supportedFileFormats(QMediaFormat::Encode)) // 支持的文件格式
        ui->comboFileFormat->addItem(QMediaFormat::fileFormatDescription(fileFormat), QVariant::fromValue(fileFormat)); // 添加用户数据
    // 采样频率
    int minSampRate = audioInput->device().minimumSampleRate(); // 输入设备支持的最低采样频率
    ui->comboSampleRate->addItem(QString("Minimun %1").arg(minSampRate), minSampRate);
    int maxSampRate = audioInput->device().maximumSampleRate(); // 输入设备支持的最高采样频率
    ui->comboSampleRate->addItem(QString("Maximum %1").arg(maxSampRate), maxSampRate);
    ui->comboSampleRate->addItem("16000", 16000); // 添加了用户数据
    ui->comboSampleRate->addItem("44100", 44100);
    ui->comboSampleRate->addItem("48000", 48000);
    ui->comboSampleRate->addItem("88200", 88200);
    // 通道数
    int minChan = audioInput->device().minimumChannelCount(); // 最少通道数
    ui->comboChannels->addItem(QString("Minimun %1").arg(minChan), minChan);
    int maxChan = audioInput->device().maximumChannelCount(); // 最多通道数
    ui->comboChannels->addItem(QString("Maximum %1").arg(maxChan), maxChan);
    ui->comboChannels->addItem("1", 1);
    ui->comboChannels->addItem("2", 2);
    // 固定品质
    ui->sliderQuality->setRange(0, int(QImageCapture::VeryHighQuality));
    ui->sliderQuality->setValue(int(QImageCapture::NormalQuality));
    // 固定比特率
    ui->comboBitrate->addItem("32000");
    ui->comboBitrate->addItem("64000");
    ui->comboBitrate->addItem("96000");
    ui->comboBitrate->addItem("128000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_stateChanged(QMediaRecorder::RecorderState state)
{
    // 录音状态变化
    bool isRecording = state == QMediaRecorder::RecordingState; // 正在录音
    ui->actRecord->setEnabled(!isRecording); // “录音”按钮
    ui->actPause->setEnabled(isRecording); // “暂停”按钮
    ui->actStop->setEnabled(isRecording); // “停止”按钮
    ui->btnGetFile->setEnabled(state == QMediaRecorder::StoppedState); // “录音输出文件”按钮
}

void MainWindow::do_durationChanged(qint64 duration)
{
    // 录音持续时间变化
    ui->labPassTime->setText(QString("已录制 %1 秒").arg(duration / 1000));
}


void MainWindow::closeEvent(QCloseEvent *event)
{
}

void MainWindow::on_actRecord_triggered()
{
    // 开始录音
    if (recorder->recorderState() == QMediaRecorder::PausedState)
    {
        // 若是暂停状态，就继续录音
        recorder->record();
        return;
    }
    QString selectedFile = ui->editOutputFile->text().trimmed();
    if (selectedFile.isEmpty()) // 检查文件
    {
        QMessageBox::critical(this, "错误", "请先设置录音输出文件");
        return;
    }
    if (QFile::exists(selectedFile))
        QFile::remove(selectedFile);
    recorder->setOutputLocation(QUrl::fromLocalFile(selectedFile)); // 设置输出文件
    // 设置session的音频输入设备
    session->audioInput()->setDevice(QMediaDevices::defaultAudioInput());

    // 设置recorder的mediaFormat参数，包括音频编码和文件格式
    QMediaFormat mediaFormat;
    QVariant var = ui->comboCodec->itemData(ui->comboCodec->currentIndex());
    QMediaFormat::FileFormat fileFormat = var.value<QMediaFormat::FileFormat>();
    mediaFormat.setFileFormat(fileFormat); // 设置文件格式

    var = ui->comboFileFormat->itemData(ui->comboFileFormat->currentIndex());
    QMediaFormat::AudioCodec audioCodec = var.value<QMediaFormat::AudioCodec>();
    mediaFormat.setAudioCodec(audioCodec); // 设置音频编码
    recorder->setMediaFormat(mediaFormat); // 设置媒介格式

    // 设置recorder的其他参数
    var = ui->comboSampleRate->itemData(ui->comboSampleRate->currentIndex());
    recorder->setAudioSampleRate(var.toInt()); // 设置采样频率
    var = ui->comboChannels->itemData(ui->comboChannels->currentIndex());
    recorder->setAudioChannelCount(var.toInt()); // 设置通道数
    recorder->setAudioBitRate(ui->comboBitrate->currentText().toInt()); // 设置比特率
    recorder->setQuality(QMediaRecorder::Quality(ui->sliderQuality->value())); // 设置录制品质
    if (ui->radioQuality->isChecked()) // 设置编码模式，固定品质
        recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);
    else // 固定比特率
        recorder->setEncodingMode(QMediaRecorder::ConstantBitRateEncoding);
    recorder->record();
}


void MainWindow::on_actPause_triggered()
{
    // 暂停
    recorder->pause();
}


void MainWindow::on_btnGetFile_clicked()
{
    //设置保存文件
    QString curPath = QDir::currentPath(); // 获取系统当前目录
    QString dlgTitle = "选择输出文件"; //对话框标题
    QString filter = "所有文件(*.*);;MP3文件(*.mp3);;WMA文件(*.wma);;MP4文件(*.mp4)"; // 文件过滤器
    QString selectedFile = QFileDialog::getSaveFileName(this, dlgTitle, curPath, filter);

    if (!selectedFile.isEmpty())
    {
        ui->editOutputFile->setText(selectedFile);
        QFileInfo fileInfo(selectedFile);
        QDir::setCurrent(fileInfo.absolutePath());
    }
}


void MainWindow::on_actStop_triggered()
{
    // 停止
    recorder->stop();
}

