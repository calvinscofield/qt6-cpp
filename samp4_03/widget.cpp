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

void Widget::on_btnCal_clicked()
{
    // “计算总价”按钮
    int num = ui->editNum->text().toInt();
    // 数量：字符串转换为整数
    float price = ui->editPrice->text().toFloat();
    // 单价：字符串转换为浮点数
    float total = num * price;
    QString str;
    str = str.setNum(total, 'f', 2); // 浮点数，有2位小数
    ui->editTotal->setText(str);
}


void Widget::on_btnDec_clicked()
{
    // 读取十进制字符串，将其转换为其他进制显示
    int val = ui->editDec->text().toInt(); // 读取十进制字符串，将其转换为整数
    QString str = QString::number(val, 16); // 显示为十六进制字符串
    str = str.toUpper(); // 转换为大写字母
    ui->editHex->setText(str);
    str = QString::number(val, 2); // 显示为二进制字符串
    ui->editBin->setText(str);
}


void Widget::on_btnBin_clicked()
{
    // 读取二进制字符串，将其转换为其他进制显示
    bool ok;
    int val = ui->editBin->text().toInt(&ok, 2); // 读取二进制字符串，将其转换为整数
    QString str = QString::number(val, 10); // 显示为十进制字符串
    ui->editDec->setText(str);
    str = QString::number(val, 16); // 显示为十六进制字符串
    str = str.toUpper(); // 转换为大写字母
    ui->editHex->setText(str);
}


void Widget::on_btnHex_clicked()
{
    // 读取十六进制字符串，将其转换为其他进制显示
    bool ok;
    int val = ui->editHex->text().toInt(&ok, 16); // 读取十六进制字符串，将其转换为整数
    QString str = QString::number(val, 10); // 显示为十进制字符串
    ui->editDec->setText(str);
    str = QString::number(val, 2); // 显示为二进制字符串
    ui->editBin->setText(str);
}


void Widget::on_btnDebug_clicked()
{
    // “qDebug()测试”按钮
    // =====setNum()函数，浮点数
    QString str;
    double num = 1245.2783;
    qDebug("num= %f", num);
    str.setNum(num, 'f', 5); // 小数点后5位，str = "1245.27830"
    qDebug("str = %s", str.toLocal8Bit().data());
    str.setNum(num, 'E', 5); // 基数的小数点后5位，str = "1.24528E+03"
    qDebug("str = %s", str.toLocal8Bit().data());
    str.setNum(num, 'g', 5); // 整数和小数总共5位，str = "1245.3"
    qDebug("str = %s", str.toLocal8Bit().data());
    str.setNum(num, 'g', 3); // 整数和小数总共3位，str = "1.25e+03"
    qDebug("str = %s", str.toLocal8Bit().data());
}

