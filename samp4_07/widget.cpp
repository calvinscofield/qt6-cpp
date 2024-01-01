#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnDebugTime_clicked()
{
    QTime TM1(13, 24, 5); // 定义变量，初始化设置时间
    QString str = TM1.toString("HH:mm:ss");
    qDebug("Qriginal time = %s", str.toLocal8Bit().data());
    QTime TM2 = TM1.addSecs(150); // 延后150秒
    str = TM2.toString("HH:mm:ss");
    qDebug("150s later, time = %s", str.toLocal8Bit().data());

    TM2 = QTime::currentTime(); // 获取当前时间
    str = TM2.toString("HH:mm:ss zzz");
    qDebug("Current time = %s", str.toLocal8Bit().data());
    qDebug("Hour = %d", TM2.hour());
    qDebug("Minute = %d", TM2.minute());
    qDebug("Second = %d", TM2.second());
    qDebug("MSecond = %d", TM2.msec());
}


void Widget::on_btnDebugDate_clicked()
{
    QDate DT1(2021, 7, 6); // 初始化日期
    QString str = DT1.toString("yyyy-MM-dd");
    qDebug("DT1 = %s", str.toLocal8Bit().data());
    QDate DT2;
    DT2.setDate(2021, 8, 25); // 设置日期
    str = DT2.toString("yyyy-MM-dd");
    qDebug("DT2 = %s", str.toLocal8Bit().data());
    qDebug("Days between DT2 and DT1 = %d", DT2.daysTo(DT1)); // DT2与DT1相差的天数

    DT2 = QDate::currentDate(); // 获取当前日期
    str = DT2.toString("yyyy-MM-dd");
    qDebug("Current date = %s", str.toLocal8Bit().data());
    qDebug("Year = %d", DT2.year());
    qDebug("Month = %d", DT2.month());
    qDebug("Day = %d", DT2.day());
    qDebug("Day of week = %d", DT2.dayOfWeek()); // 1表示星期一，7表示星期天
}


void Widget::on_btnDebugDateTime_clicked()
{
    QDateTime DT1 = QDateTime::currentDateTime(); // 系统当前日期时间
    QString str = DT1.toString("yyyy-MM-dd hh:mm:ss");
    qDebug("DT1 = %s", str.toLocal8Bit().data());

    QDate dt = DT1.date(); // 日期部分
    str = dt.toString("yyyy-MM-dd");
    qDebug("DT1.date() = %s", str.toLocal8Bit().data());
    QTime tm = DT1.time(); // 时间部分
    str = tm.toString("hh:mm:ss zzz");
    qDebug("DT1.time() = %s", str.toLocal8Bit().data());

    qint64 MS = DT1.toSecsSinceEpoch(); // 转换为秒数
    qDebug("DT1.toSecsSinceEpoch() = %lld", MS);
    MS += 120;
    DT1.setSecsSinceEpoch(MS); // 加120秒以后
    str = DT1.toString("yyyy-MM-dd hh:mm:ss");
    qDebug("DT1 + 120s = %s", str.toLocal8Bit().data());
}


void Widget::on_btnGetTime_clicked()
{
    // “读取当前日期时间”按钮
    QDateTime curDateTime = QDateTime::currentDateTime(); // 读取当前日期时间
    ui->timeEdit->setTime(curDateTime.time()); // 设置时间
    ui->editTime->setText(curDateTime.toString("hh:mm:ss"));
    ui->dateEdit->setDate(curDateTime.date()); // 设置日期
    ui->editDate->setText(curDateTime.toString("yyyy-MM-dd"));
    ui->dateTimeEdit->setDateTime(curDateTime); // 设置日期时间
    ui->editDateTime->setText(curDateTime.toString("yyyy-MM-dd hh:mm:ss"));
}


void Widget::on_btnSetTime_clicked()
{
    // “设置时间fromString”按钮
    QString str = ui->editTime->text(); // 将取字符串表示的时间数据
    str = str.trimmed(); // 去掉可能的多余空格
    if (!str.isEmpty())
    {
        QTime tm = QTime::fromString(str, "hh:mm:ss"); // 将字符串转换为QTime数据
        ui->timeEdit->setTime(tm); // 设置时间
    }
}


void Widget::on_btnSetDate_clicked()
{
    // “设置日期fromString”按钮
    QString str = ui->editDate->text(); // 读取字符串表示的日期
    str = str.trimmed();
    if (!str.isEmpty())
    {
        QDate dt = QDate::fromString(str, "yyyy-MM-dd"); // 将从字符串转换为QDate数据
        ui->dateEdit->setDate(dt); // 设置日期
    }
}


void Widget::on_btnSetDateTime_clicked()
{
    // “日期时间fromString”按钮
    QString str = ui->editDateTime->text(); // 读取字符串表示的日期时间
    str = str.trimmed();
    if (!str.isEmpty())
    {
        QDateTime datetime = QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss"); // 将从字符串转换为QDate数据
        ui->dateTimeEdit->setDateTime(datetime); // 设置日期时间
    }
}


void Widget::on_calendarWidget_selectionChanged()
{
    // 日历组件的selectionChanged()信号的槽函数
    QDate dt = ui->calendarWidget->selectedDate(); // 读取选择的日期时间
    QString str = dt.toString("yyyy年M月d日");
    ui->editCalendar->setText(str);
}

