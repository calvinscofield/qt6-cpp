#include "tgraphicsview.h"

#include <QMouseEvent>

TGraphicsView::TGraphicsView(QWidget *parent) : QGraphicsView(parent) {}


void TGraphicsView::mousePressEvent(QMouseEvent *event)
{
    // 鼠标左键按下事件
    if (event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos(); // QGraphicsView的坐标
        emit mouseClicked(point); // 发射信号
    }
    QGraphicsView::mousePressEvent(event);
}

void TGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标移动事件
    QPoint point = event->pos(); // QGraphicsView的坐标
    emit mouseMovePoint(point); // 发射信号
    QGraphicsView::mouseMoveEvent(event);
}


void TGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 鼠标双击事件
    if (event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos(); // QGraphicsView的坐标
        emit mouseDoubleClick(point); // 发射信号
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void TGraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    // 按键事件
    emit keyPress(event); // 发射信号
    QGraphicsView::keyPressEvent(event);
}
