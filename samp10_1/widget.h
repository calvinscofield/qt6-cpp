#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    void myDrawFilledRect(); // 填充矩形框
    void myDrawTextureRect(); // 材质填充绘图
    void myDrawGradient(); // 3种渐变
    void myDrawGradientSpread(); // 延展渐变
    void myDrawShape(); // 绘制基本图形
    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};
#endif // WIDGET_H
