#ifndef TGRAPHICSVIEW_H
#define TGRAPHICSVIEW_H

#include <QGraphicsView>

class TGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    TGraphicsView(QWidget *parent = nullptr);
signals:
    void mouseMovePoint(QPoint point); // 鼠标移动
    void mouseClicked(QPoint point); // 鼠标单机
    void mouseDoubleClick(QPoint point); // 双击事件
    void keyPress(QKeyEvent *event); // 按键事件

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // TGRAPHICSVIEW_H
