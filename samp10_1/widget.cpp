#include "widget.h"
#include "ui_widget.h"

#include <QPaintEvent>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setPalette(QPalette(Qt::white)); // 设置窗口为白色背景
    setAutoFillBackground(true);
    this->resize(400, 300);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::myDrawFilledRect()
{
    QPainter painter(this); // 创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    int W = this->width(); // 绘图区宽度
    int H = this->height(); // 绘图区高度
    QRect rect(W / 4, H / 4, W / 2, H / 2); // 中间区域矩形
    // 设置画笔
    QPen pen;
    pen.setWidth(3); // 线宽
    pen.setColor(Qt::red); // 线条颜色
    pen.setStyle(Qt::SolidLine); // 线条样式
    pen.setCapStyle(Qt::FlatCap); // 线条端点样式
    pen.setJoinStyle(Qt::BevelJoin); // 线条的连接样式
    painter.setPen(pen);
    // 设置画刷
    QBrush brush;
    brush.setColor(Qt::yellow); // 画刷颜色
    brush.setStyle(Qt::SolidPattern); // 画刷填充样式
    painter.setBrush(brush);
    // 绘图
    painter.drawRect(rect); // 绘制矩形
}

void Widget::myDrawTextureRect()
{
    QPainter painter(this); // 创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);
    int W = this->width(); // 绘图区宽度
    int H = this->height(); // 绘图区高度
    QRect rect(W / 4, H / 4, W / 2, H / 2); // 中间区域矩形
    // 设置画笔
    QPen pen;
    pen.setWidth(3); // 线宽
    pen.setColor(Qt::red); // 线条颜色
    pen.setStyle(Qt::SolidLine); // 线条样式
    pen.setCapStyle(Qt::FlatCap); // 线条端点样式
    pen.setJoinStyle(Qt::BevelJoin); // 线条的连接样式
    painter.setPen(pen);
    // 设置画刷
    QPixmap texturePixmap(":/images/images/texture.jpg");
    QBrush brush;
    brush.setStyle(Qt::TexturePattern); // 画刷填充样式
    brush.setTexture(texturePixmap); // 设置材质图片
    painter.setBrush(brush);
    // 绘图
    painter.drawRect(rect); // 绘制矩形
}

void Widget::myDrawGradient()
{
    QPainter painter(this);
    int W = this->width(); // 绘图区宽度
    int H = this->height(); // 绘图区高度
    QRect rect(W / 4, H / 4, W / 2, H / 2); // 中间区域矩形
    painter.setPen(QPen(Qt::NoPen)); // 设置画笔，不显示线条
    // 线性渐变
    QLinearGradient linearGrad(rect.left(), rect.top(), rect.right(), rect.top()); // 从左到右
    linearGrad.setColorAt(0, Qt::blue); // 起点颜色
    linearGrad.setColorAt(0.5, Qt::white); // 中间点颜色
    linearGrad.setColorAt(1, Qt::blue); // 终点颜色
    painter.setBrush(linearGrad);
    painter.drawRect(rect); // 只填充定义的渐变区域

    // 辐射渐变
    // QRadialGradient radialGrad(W / 2, H / 2, qMax(W / 3, H / 3), W / 2, H / 2);
    // radialGrad.setColorAt(0, Qt::white);
    // radialGrad.setColorAt(1, Qt::blue);
    // painter.setBrush(radialGrad);
    // painter.drawRect(rect); // 只填充定义的渐变区域


    // 径向渐变，焦点不同
    // QRadialGradient radialGrad(W / 2, H / 2, W / 2, 3 * H / 4, H / 2);
    // radialGrad.setColorAt(0, Qt::yellow);
    // radialGrad.setColorAt(0.8, Qt::blue);
    // painter.setBrush(radialGrad);
    // painter.drawEllipse(rect);

    // 圆锥形渐变
    // QConicalGradient coniGrad(W / 2, H / 2, 45);
    // coniGrad.setColorAt(0, Qt::blue);
    // coniGrad.setColorAt(1, Qt::white);
    // painter.setBrush(coniGrad);
    // painter.drawRect(rect); // 只填充定义的渐变区域
}

void Widget::myDrawGradientSpread()
{
    QPainter painter(this);
    int W = this->width();
    int H = this->height();
    painter.setPen(QPen(Qt::NoPen)); // 设置画笔，不显示线条
    // 辐射渐变
    QRadialGradient radialGrad(W / 2, H / 2, qMax(W / 8, H / 8), W / 2, H / 2);
    radialGrad.setColorAt(0, Qt::yellow);
    radialGrad.setColorAt(1, Qt::blue);
    radialGrad.setSpread(QGradient::PadSpread); // 延展方式还有RepeatSpread、ReflectSpread
    painter.setBrush(radialGrad);
    painter.drawRect(this->rect()); // 填充更大区域，会有延展效果
}

void Widget::myDrawShape()
{
    QPainter painter(this); // 创建QPainter对象
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int W = this->width(); // 绘图区宽度
    int H = this->height(); // 绘图区高度

    //设置画笔
    QPen pen;
    pen.setWidth(3); // 线宽
    pen.setColor(Qt::black); // 划线颜色

    // Qt::NoPen,Qt::SolidLine, Qt::DashLine, Qt::DotLine,Qt::DashDotLine,Qt::DashDotDotLine,Qt::CustomDashLine
    pen.setStyle(Qt::SolidLine); // 线的类型，实线、虚线等

    // Qt::FlatCap, Qt::SquareCap,Qt::RoundCap
    pen.setCapStyle(Qt::FlatCap); // 线端点样式

    // Qt::MiterJoin,Qt::BevelJoin,Qt::RoundJoin,Qt::SvgMiterJoin
    pen.setJoinStyle(Qt::BevelJoin); // 线的连接点样式
    painter.setPen(pen);

    // 1. drawPoint()绘制一个点
    painter.drawPoint(QPoint(W / 2, H / 2));

    // // 2. drawPoints()绘制一批点
    // QPoint points[] = {
    //     QPoint(5 * W / 12, H / 4),
    //     QPoint(3 * W / 4, 5 * H / 12),
    //     QPoint(2 * W / 4, 5 * H / 12)
    // };
    // painter.drawPoints(points, 3);

    // // 3. drawLine()绘制直线
    // QLine Line(W / 4, H / 4, W / 2, H / 2);
    // painter.drawLine(Line);

    // // 4. drawLines()绘制一批直线
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // QList<QLine> Lines;
    // Lines.append(QLine(rect.topLeft(), rect.bottomRight()));
    // Lines.append(QLine(rect.topRight(), rect.bottomLeft()));
    // Lines.append(QLine(rect.topLeft(), rect.bottomLeft()));
    // Lines.append(QLine(rect.topRight(), rect.bottomRight()));
    // painter.drawLines(Lines);

    // // 5. drawArc()绘制弧线
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // int startAngle = 90 * 16; // 起始90°
    // int spanAngle = 90 * 16; // 旋转90°
    // painter.drawArc(rect, startAngle, spanAngle);

    // // 6. drawChord()绘制一段弦
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // int startAngle = 90 * 16; // 起始90°
    // int spanAngle = 90 * 16; // 旋转90°
    // painter.drawChord(rect, startAngle, spanAngle);

    // // 7. drawPie()绘制扇形
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // int startAngle = 40 * 16; // 起始40°
    // int spanAngle = 120 * 16; // 旋转120°
    // painter.drawPie(rect, startAngle, spanAngle);

    // // 8. drawConvexPolygon()根据给定的点绘制凸多边形
    // QPoint points[4] = {
    //     QPoint(5 * W / 12, H / 4),
    //     QPoint(3 * W / 4, 5 * H / 12),
    //     QPoint(5 * W / 12, 3 * H / 4),
    //     QPoint(W / 4, 5 * H / 12)
    // };
    // painter.drawConvexPolygon(points, 4);

    // // 9. drawPolygon()绘制多边形，最后一个点会和第一个点重合
    // QPoint points[] = {
    //     QPoint(5 * W / 12, H / 4),
    //     QPoint(3 * W / 4, 5 * H / 12),
    //     QPoint(5 * W / 12, 3 * H / 4),
    //     QPoint(2 * W / 4, 5 * H / 12)
    // };
    // painter.drawPolygon(points, 4);

    // // 10. drawPolyline()绘制多点连接的线，最后一个点不会和第一个点连接
    // QPoint points[] = {
    //     QPoint(5 * W / 12, H / 4),
    //     QPoint(3 * W / 4, 5 * H / 12),
    //     QPoint(5 * W / 12, 3 * H / 4),
    //     QPoint(2 * W / 4, 5 * H / 12)
    // };
    // painter.drawPolyline(points, 4);

    // // 11. drawImage()将QImage对象存储的图片绘制在指定的矩形区域内
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // QImage image(":images/images/qt.jpg");
    // painter.drawImage(rect, image);

    // // 12. drawPixmap()将QPixmap对象存储的图片绘制在指定的矩形区域内
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // QPixmap image(":images/images/qt.jpg");
    // painter.drawPixmap(rect, image);

    // // 13. drayText()绘制文本，只能绘制单行文字，字体属性由QPainter::font()决定
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // QFont font;
    // font.setPixelSize(30);
    // font.setBold(true);
    // painter.setFont(font);
    // painter.drawText(rect, "Hello, Qt");

    // // 14. drawEllipse()绘制椭圆
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // painter.drawEllipse(rect);


    // // 15. drawRect()绘制矩形
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // painter.drawRect(rect);

    // // 16. drawRoundedRect()绘制圆角矩形
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // painter.drawRoundedRect(rect, 20, 20);

    // // 17. fillRect()填充矩形，无边框线
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // painter.fillRect(rect, Qt::green);

    // // 18. eraseRect()擦除某个矩形区域，等效于用背景色填充该区域
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // painter.eraseRect(rect);

    // // 19. drawPath()绘制由QPainterPath对象定义的路径
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // QPainterPath path;
    // path.addEllipse(rect);
    // path.addRect(rect);
    // painter.drawPath(path);

    // // 20. fillPath()填充某个QPainterPath对象定义的绘图路径，但是不显示轮廓
    // QRect rect(W / 4, H / 4, W / 2, H / 2);
    // QPainterPath path;
    // path.addEllipse(rect);
    // path.addRect(rect);
    // painter.fillPath(path, Qt::red);
}

void Widget::paintEvent(QPaintEvent *event)
{
    // 1. 基本绘图1，绘制一个填充矩形
    myDrawFilledRect();

    // // 2. 用材质图片填充一个矩形
    // myDrawTextureRect();

    // // 3. 渐变填充
    // myDrawGradient();
    // myDrawGradientSpread(); // 延展渐变

    // // 4. 绘制基本图形
    // myDrawShape();

    event->accept();
}
