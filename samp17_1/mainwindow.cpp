#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

void MainWindow::uartSend(QString cmd)
{
    ui->textCOM->appendPlainText(cmd);
    const char *stdCmd = cmd.toLocal8Bit().data(); // 转换为 char*类型字符串
    // QByteArray bts = cmd.toLocal8Bit(); // 转换为 8 位字符数据数组
    comPort.write(stdCmd);
}

void MainWindow::delayMs(int ms)
{
    // 延时数据的单位：ms
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < ms)
        QApplication::processEvents();
}

void MainWindow::loadFromReg()
{
    QSettings setting;
    bool saved = setting.value("saved", false).toBool();
    if (!saved)
        return;
    // 查找 frame_CmdA 中的 QLineEdit 对象，载入注册表里的数据
    QList<QLineEdit*> editList = ui->frame_CmdA->findChildren<QLineEdit*>();
    foreach (QLineEdit *edit, editList)
    {
        QString editName = edit->objectName();
        edit->setText(setting.value(editName).toString());
    }
    // 查找 frame_CmdB 中的 QLineEdit 对象，载入注册表里的数据
    editList = ui->frame_CmdB->findChildren<QLineEdit*>();
    foreach (QLineEdit *edit, editList)
    {
        QString editName = edit->objectName();
        edit->setText(setting.value(editName).toString());
    }
    ui->comboWiFi_UartBuad->setCurrentText(setting.value("Uart_Rate").toString());
    ui->editAP_Name->setText(setting.value("AP_Name").toString());
    ui->editAP_PWD->setText(setting.value("AP_PWD").toString());
    ui->editServer_IP->setText(setting.value("TCP_IP").toString());
    ui->spinServer_Port->setValue(setting.value("TCP_Port").toInt());
}

void MainWindow::saveToReg()
{
    // QSettings  setting(QApplication::organizationName(), QApplication::applicationName());
    QSettings  setting;
    setting.setValue("saved", true);

    QList<QLineEdit*> editList = ui->frame_CmdA->findChildren<QLineEdit*>();
    foreach (QLineEdit *edit, editList)
    {
        QString editName = edit->objectName();
        setting.setValue(editName, edit->text());
    }

    editList = ui->frame_CmdB->findChildren<QLineEdit*>();
    foreach (QLineEdit *edit, editList)
    {
        QString editName = edit->objectName();
        setting.setValue(editName, edit->text());
    }

    setting.setValue("Uart_Rate", ui->comboWiFi_UartBuad->currentText());
    setting.setValue("AP_Name", ui->editAP_Name->text());
    setting.setValue("AP_PWD", ui->editAP_PWD->text());
    setting.setValue("TCP_IP", ui->editServer_IP->text());
    setting.setValue("TCP_Port", ui->spinServer_Port->value());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->toolBox->setEnabled(false); // 先禁止操作
    ui->tabWidget->setEnabled(false);
    // 显示串口列表
    foreach (QSerialPortInfo portInfo, QSerialPortInfo::availablePorts())
        ui->comboCom_Port->addItem(portInfo.portName() + ":" + portInfo.description());
    ui->actCom_Open->setEnabled(ui->comboCom_Port->count() > 0);
    connect(&comPort, &QIODevice::readyRead, this, &MainWindow::do_com_readyRead);
    // 添加标准波特率
    ui->comboCom_Buad->clear();
    foreach (qint32 baud, QSerialPortInfo::standardBaudRates())
        ui->comboCom_Buad->addItem(QString::number(baud));
    ui->comboCom_Buad->setCurrentText("115200"); // 默认使用 115200
    // 为 frame_CmdA 和 frame_CmdB 里面的 TMyLabel 标签的 clicked()信号关联槽函数
    QList<TMyLabel*> labList = ui->frame_CmdA->findChildren<TMyLabel*>();
    foreach (TMyLabel *lab, labList)
        connect(lab, SIGNAL(clicked()), this, SLOT(do_label_clicked()));
    labList = ui->frame_CmdB->findChildren<TMyLabel*>();
    foreach (TMyLabel *lab, labList)
        connect(lab, SIGNAL(clicked()), this, SLOT(do_label_clicked()));
    QApplication::setOrganizationName("WWB-Qt"); // 设置应用程序参数，用于注册表
    QApplication::setApplicationName("ESP8266");
    loadFromReg(); // 从注册表载入数
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_com_readyRead()
{
    QByteArray all = comPort.readAll();
    QString str(all);
    ui->textCOM->appendPlainText(str);
}

void MainWindow::do_label_clicked()
{
    TMyLabel *lab = static_cast<TMyLabel*>(sender()); // 获取信号发射者
    QLineEdit *edit = static_cast<QLineEdit*>(lab->buddy()); // 获取伙伴组件
    QString cmd = edit->text().trimmed(); // 指令字符串
    if (ui->chkBox_NewLine2->isChecked())
        cmd = cmd + "\r\n"; // 添加回车换行符
    uartSend(cmd); // 通过串口发送字符串
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (comPort.isOpen()) // 确保串口被关闭
        comPort.close();
    saveToReg(); // 保存到注册表
    event->accept();
}

void MainWindow::on_actCom_Open_triggered()
{
    // “打开串口”按钮
    if (comPort.isOpen())
    {
        QMessageBox::warning(this, "错误", "打开串口时出现错误");
        return;
    }
    QList<QSerialPortInfo> comList = QSerialPortInfo::availablePorts();
    QSerialPortInfo portInfo = comList.at(ui->comboCom_Port->currentIndex());
    comPort.setPort(portInfo); // 设置使用哪个串口
    // comPort.setPortName(portInfo.portName()); // 也可以设置串口名称
    // 设置串口通信参数
    QString str = ui->comboCom_Buad->currentText();
    comPort.setBaudRate(str.toInt()); // 设置波特率
    int value = ui->comboCom_DataBit->currentText().toInt();
    comPort.setDataBits(QSerialPort::DataBits(value)); // 数据位，默认为 8 位
    value = 1 + ui->comboCom_StopBit->currentIndex();
    comPort.setStopBits(QSerialPort::StopBits(value)); // 停止位，默认为 1 位
    if (ui->comboCom_Parity->currentIndex() == 0)
        value = 0;
    else
        value = 1 + ui->comboCom_Parity->currentIndex();
    comPort.setParity(QSerialPort::Parity(value)); // 校验位，默认为无
    if (comPort.open(QIODeviceBase::ReadWrite))
    {
        ui->frame_Port->setEnabled(false); // 串口设置面板
        ui->actCom_Open->setEnabled(false);
        ui->actCom_Close->setEnabled(true);
        ui->toolBox->setEnabled(true); // ESP8266 设置操作面板
        ui->tabWidget->setEnabled(true);
        QMessageBox::information(this, "提示信息", "串口已经被成功打开");
    }
}


void MainWindow::on_actCom_Close_triggered()
{
    // “关闭串口”按钮
    if (comPort.isOpen())
    {
        comPort.close();
        ui->frame_Port->setEnabled(true);
        ui->actCom_Open->setEnabled(true);
        ui->actCom_Close->setEnabled(false);
        ui->toolBox->setEnabled(false);
        ui->tabWidget->setEnabled(false);
    }
}


void MainWindow::on_btnWF_SetAP_clicked()
{
    QString APname = ui->editAP_Name->text().trimmed(); // 网络名称
    QString APpswd = ui->editAP_PWD->text().trimmed(); // 密码
    QString cmd = "AT+CWJAP=\"" + APname + "\"," + "\"" + APpswd+ "\"\r\n";
    uartSend(cmd);
}


void MainWindow::on_btnTrans_Enter_clicked()
{
    // “进入透传模式”按钮
    QString cmd = "AT+CIPMODE=1\r\n";
    uartSend(cmd); // 发送第一个指令
    delayMs(2000); // 延时 2000ms
    cmd = "AT+CIPSEND\r\n";
    uartSend(cmd); // 发送第二个指令
}

