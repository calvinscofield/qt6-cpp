#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->spinNum, SIGNAL(valueChanged(int)), this, SLOT(on_btnCal_clicked()));
    connect(ui->spinPrice, SIGNAL(valueChanged(double)), this, SLOT(on_btnCal_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnCal_clicked()
{
    // “计算”按钮
    int num = ui->spinNum->value(); // 读取数量，是整数
    float price = ui->spinPrice->value(); // 读取单价，是浮点数
    float total = num * price;
    ui->spinTotal->setValue(total); // 直接显示浮点数
}


void Widget::on_spinDec_valueChanged(int arg1)
{
    // “十进制”SpinBox
    qDebug(ui->spinDec->cleanText().toLocal8Bit().data()); // 显示数值的十进制字符串
    ui->spinBin->setValue(arg1); // 设置整数，自动以二进制显示
    ui->spinHex->setValue(arg1); // 设置整数，自动以十六进制显示
}


void Widget::on_spinBin_valueChanged(int arg1)
{
    // “二进制”SpinBox
    qDebug(ui->spinBin->cleanText().toLocal8Bit().data()); // 显示数值的二进制字符串
    ui->spinDec->setValue(arg1);
    ui->spinHex->setValue(arg1);
}


void Widget::on_spinHex_valueChanged(int arg1)
{
    // “十六进制”SpinBox
    qDebug(ui->spinHex->cleanText().toLocal8Bit().data()); // 显示数值的十六进制字符串
    ui->spinDec->setValue(arg1);
    ui->spinBin->setValue(arg1);
}

