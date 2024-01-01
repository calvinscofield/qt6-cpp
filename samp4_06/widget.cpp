#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 将表盘、滑动条、卷滚条的valueChanged()信号与自定义槽函数关联
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(do_valueChanged(int)));
    connect(ui->scrollBar, SIGNAL(valueChanged(int)), this, SLOT(do_valueChanged(int)));
    connect(ui->dial, SIGNAL(valueChanged(int)), this, SLOT(do_valueChanged(int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::do_valueChanged(int value)
{
    // 自定义槽函数
    ui->progressBar->setValue(value); // 设置进度条的value
}

void Widget::on_chkBox_Visible_clicked(bool checked)
{
    // textVisible复选框
    ui->progressBar->setTextVisible(checked);
}


void Widget::on_chkBox_Inverted_clicked(bool checked)
{
    // InvertedAppearance复选框
    ui->progressBar->setInvertedAppearance(checked);
}


void Widget::on_radio_Percent_clicked()
{
    // “显示格式--百分比”单选按钮
    ui->progressBar->setFormat("%p%"); // 进度条显示百分比
}


void Widget::on_radio_Value_clicked()
{
    // “显示格式--当前值”单选按钮
    ui->progressBar->setFormat("%v"); // 进度条显示数值
}

