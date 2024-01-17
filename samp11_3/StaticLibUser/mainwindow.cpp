#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPaintEvent>
#include <QPainter>
#include <TPenDialog.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    // 窗口的绘图事件处理函数
    QPainter painter(this);
    QRect rect(0, 0, width(), height()); // 视口矩形区
    painter.setViewport(rect); // 设置视口
    painter.setWindow(0, 0, 100, 50); // 设置窗口大小，逻辑坐标
    painter.setPen(m_Pen);
    painter.drawRect(10, 10, 80, 30);
    event->accept();
}

void MainWindow::on_action_Pen_triggered()
{
    // “设置Pen”按钮
    bool ok = false;
    QPen pen = TPenDialog::getPen(m_Pen, &ok);
    if (ok)
    {
        m_Pen = pen;
        this->repaint();
    }
}

