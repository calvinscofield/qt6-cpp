#include "tpbattery.h"
#include <QPaintEvent>
#include <QPainter>
TPBattery::TPBattery(QWidget *parent) :
    QWidget(parent)
{
}

int TPBattery::powerLevel()
{
    // 返回当前电量值
    return m_powerLevel;
}

void TPBattery::setPowerLevel(int pow)
{
    // 设置当前电量值
    m_powerLevel = pow;
    emit powerLevelChanged(pow); // 发射信号
    repaint();
}

int TPBattery::warnLevel()
{
    // 返回电量低阈值
    return m_warnLevel;
}

void TPBattery::setWarnLevel(int warn)
{
    // 设置电量低阈值
    m_warnLevel = warn;
    repaint();
}

void TPBattery::paintEvent(QPaintEvent *event)
{
    // 绘制界面组件
    QPainter painter(this);
    QRect rect(0, 0, width(), height()); // 视口矩形区
    painter.setViewport(rect); // 设置视口
    painter.setWindow(0, 0, 120, 50); // 设置窗口大小，逻辑坐标
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // 绘制电池边框
    QPen pen(colorBorder); // 边框线条颜色
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);
    QBrush brush(colorBack); // 画刷颜色
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    rect.setRect(1, 1, 109, 48);
    painter.drawRect(rect); // 绘制电池边框
    brush.setColor(colorBorder);
    painter.setBrush(brush);
    rect.setRect(110, 15, 10, 20);
    painter.drawRect(rect); // 画电池正极头

    // 画电量柱
    if (m_powerLevel > m_warnLevel) // 正常颜色的电量柱
    {
        brush.setColor(colorPower);
        pen.setColor(colorPower);
    }
    else // 电量低的电量柱
    {
        brush.setColor(colorWarning);
        pen.setColor(colorWarning);
    }
    painter.setBrush(brush);
    painter.setPen(pen);
    if (m_powerLevel > 0)
    {
        rect.setRect(5, 5, m_powerLevel, 40);
        painter.drawRect(rect); // 画电量柱
    }

    // 绘制电量百分比文字
    QFontMetrics textSize(this->font());
    QString powStr = QString::asprintf("%d%%", m_powerLevel);
    QRect textRect = textSize.boundingRect(powStr); // 得到字符串的矩形区
    painter.setFont(this->font());
    pen.setColor(colorBorder);
    painter.setPen(pen);
    painter.drawText(55 - textRect.width() / 2, 23 + textRect.height() / 2, powStr);
    event->accept(); // 表示事件已处理
}


QSize TPBattery::sizeHint() const
{
    // 设置组件的合适大小
    int H = this->height();
    int W = H * 12 / 5;
    QSize size(W, H);
    return size;
}
