#include "widget.h"
#include "ui_widget.h"

#include <QTime>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // “定时器”和“定时器精度”分组框在垂直方向上设置为固定尺寸
    // ui->groupBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    // ui->groupBox_3->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    //【勘误】在界面设计器里已经默认是垂直固定了，没必要再设置

    m_timer = new QTimer(this); // 创建定时器
    m_timer->stop(); // 先停止定时器
    m_timer->setTimerType(Qt::CoarseTimer); // 定时器精度
    ui->radioCoarse->setChecked(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(do_timer_timeout()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::do_timer_timeout()
{
    // 与定时器的timeout()信号关联的槽函数
    QApplication::beep(); // 使系统“嘀”一声
    QTime curTime = QTime::currentTime(); // 获取当前时间
    ui->LCDHour->display(curTime.hour()); // LCD显示 小时
    ui->LCDMin->display(curTime.minute()); // LCD显示 分钟
    ui->LCDSec->display(curTime.second()); // LCD显示 秒
    if (m_timer->isSingleShot()) // 如果是单次定时，显示流逝的时间
    {
        int tmMsec = m_counter.elapsed(); // 毫秒数
        QString str = QString("流逝的时间：%1毫秒").arg(tmMsec);
        ui->labElapsedTime->setText(str);
        ui->btnStart->setEnabled(true);
        ui->btnOneShot->setEnabled(true);
        ui->btnStop->setEnabled(false);
    }
}

void Widget::do_timer_shot()
{
    // 与动态创建的单次定时器的timeout()信号关联的槽函数
    QApplication::beep();
    int tmMsec = m_counter.elapsed(); // 流逝的时间，单位：毫秒
    QString str = QString("流逝的时间：%1毫秒").arg(tmMsec);
    ui->labElapsedTime->setText(str);
    ui->btnOneShot->setEnabled(true);
}

void Widget::on_btnStart_clicked()
{
    // “开始”按钮
    m_timer->setInterval(ui->spinBoxIntv->value()); // 设置定时器的周期
    if (ui->radioContiue->isChecked())
        m_timer->setSingleShot(false); // 设置为连续定时
    else
        m_timer->setSingleShot(true); // 设置为单词定时
    // 设置定时器精度
    if (ui->radioPrecise->isChecked())
        m_timer->setTimerType(Qt::PreciseTimer);
    else if (ui->radioCoarse->isChecked())
        m_timer->setTimerType(Qt::CoarseTimer);
    else
        m_timer->setTimerType(Qt::VeryCoarseTimer);

    m_timer->start(); // 启动定时器
    m_counter.start(); // 启动计时器
    ui->btnStart->setEnabled(false);
    ui->btnOneShot->setEnabled(false);
    ui->btnStop->setEnabled(true);
}

void Widget::on_btnStop_clicked()
{
    // “停止”按钮
    m_timer->stop(); // 定时器停止
    int tmMsec = m_counter.elapsed(); // 流逝的时间，单位：毫秒
    int ms = tmMsec % 1000; // 余数，单位：毫秒
    int sec = tmMsec / 1000; // 单位：整秒
    QString str = QString("流逝的时间：%1秒，%2毫秒").arg(sec).arg(ms, 3, 10, QChar('0'));
    ui->labElapsedTime->setText(str);
    ui->btnStart->setEnabled(true);
    ui->btnOneShot->setEnabled(true);
    ui->btnStop->setEnabled(false);
}


void Widget::on_btnOneShot_clicked()
{
    // “动态创建单次定时器”按钮
    int intv = ui->spinBoxIntv->value(); // 定时周期
    QTimer::singleShot(intv, Qt::PreciseTimer, this, &Widget::do_timer_shot);
    m_counter.start(); // 启动计时器
    ui->btnOneShot->setEnabled(false);
}

