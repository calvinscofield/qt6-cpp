#ifndef TCHARTVIEW_H
#define TCHARTVIEW_H

#include <QChartView>
#include <QWidget>

class TChartView : public QChartView
{
    Q_OBJECT
private:
    QPoint beginPoint; // 选择矩形区域的起点
    QPoint endPoint; // 选择矩形区域的终点
    bool m_customZoom = false; // 是否使用自定义矩形放大模式
public:
    TChartView(QWidget *parent = nullptr);
    void setCustomZoomRect(bool custom); // 设置是否使用自定义矩形放大模式

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event); // 鼠标左键被按下
    void mouseReleaseEvent(QMouseEvent *event); // 鼠标左键被释放
    void mouseMoveEvent(QMouseEvent *event); // 鼠标移动
    void wheelEvent(QWheelEvent *event); // 鼠标滚轮事件，缩放
    void keyPressEvent(QKeyEvent *event); // 按键事件
signals:
    void mouseMovePoint(QPoint point); // 鼠标移动信号
};

#endif // TCHARTVIEW_H
