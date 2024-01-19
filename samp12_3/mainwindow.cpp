#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::createChart()
{
    // 创建图表
    chart = new QChart();
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartView->setCursor(Qt::CrossCursor); // 设置鼠标光标为十字形

    QLineSeries *series0 = new QLineSeries();
    series0->setName("LineSeries曲线");
    series0->setPointsVisible(true); // 显示数据点
    series0->setMarkerSize(5); // 数据点大小
    series0->setSelectedColor(Qt::blue); // 选中点的颜色
    connect(series0, &QLineSeries::clicked, this, &MainWindow::do_series_clicked);
    connect(series0, &QLineSeries::hovered, this, &MainWindow::do_series_hovered);

    QSplineSeries *series1 = new QSplineSeries();
    series1->setName("SplineSeries曲线");
    series1->setPointsVisible(true);
    series1->setMarkerSize(5);
    series1->setSelectedColor(Qt::blue); // 选中点的颜色
    connect(series1, &QSplineSeries::clicked, this, &MainWindow::do_series_clicked);
    connect(series1, &QSplineSeries::hovered, this, &MainWindow::do_series_hovered);

    QPen pen(Qt::black);
    pen.setStyle(Qt::DotLine); // 虚线
    pen.setWidth(2);
    series0->setPen(pen);
    pen.setStyle(Qt::SolidLine); // 实线
    series1->setPen(pen);
    chart->addSeries(series0);
    chart->addSeries(series1);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%.1f"); // 标签格式
    axisX->setTickCount(11); // 主刻度个数
    axisX->setMinorTickCount(2);
    axisX->setTitleText("time(secs)");

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-2, 2);
    axisY->setLabelFormat("%.2f"); // 标签格式
    axisY->setTickCount(5);
    axisY->setMinorTickCount(2);
    axisY->setTitleText("value");

    chart->addAxis(axisX, Qt::AlignBottom); // 坐标轴添加到图表中，并指定方向
    chart->addAxis(axisY, Qt::AlignLeft);
    series0->attachAxis(axisX); // 序列series0附加到坐标轴
    series0->attachAxis(axisY);
    series1->attachAxis(axisX); // 序列series1附加到坐标轴
    series1->attachAxis(axisY);
    foreach (QLegendMarker *marker, chart->legend()->markers())
        connect(marker, SIGNAL(clicked()), this, SLOT(do_legendMarkerClicked()));
}

void MainWindow::prepareData()
{
    // 为序列生成数据
    QLineSeries *series0 = (QLineSeries *)chart->series().at(0);
    QSplineSeries *series1 = (QSplineSeries *)chart->series().at(1);
    qreal t = 0, y1, y2, intv = 0.5;
    int cnt = 21;
    for (int i = 0; i < cnt; i++)
    {
        int rd = QRandomGenerator::global()->bounded(-5, 6); // 随机整数，[-5,5]
        y1 = qSin(2 * t) + rd / 50;
        series0->append(t, y1);
        rd = QRandomGenerator::global()->bounded(-5, 6); // 随机整数，[-5,5]
        y2 = qSin(2 * t + 20) + rd / 50;
        series1->append(t, y2);
        t += intv;
    }
}

int MainWindow::getIndexFromX(QXYSeries *series, qreal xValue, qreal tol)
{
    QList<QPointF> points = series->points(); // 返回数据点的列表
    int index = -1;
    for (int i = 0; i < points.count(); i++)
    {
        qreal dx = xValue - points.at(i).x();
        if (qAbs(dx) <= tol)
        {
            index = i;
            break;
        }
    }
    return index; // -1表示没有找到
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // calvin注释：书中有这一行，不需要这一行代码，应该注释掉。
    // this->setCentralWidget(ui->chartView);
    lab_chartXY = new QLabel("Chart X=,  Y=  "); // 用于添加到状态栏的QLabel组件
    lab_chartXY->setMinimumWidth(200);
    ui->statusBar->addWidget(lab_chartXY);
    lab_hoverXY = new QLabel("Hovered X=,  Y=  ");
    lab_hoverXY->setMinimumWidth(200);
    ui->statusBar->addWidget(lab_hoverXY);
    lab_clickXY = new QLabel("Clicked X=,  Y=  ");
    lab_clickXY->setMinimumWidth(200);
    ui->statusBar->addWidget(lab_clickXY);
    createChart(); // 创建图表
    prepareData(); // 生成数据
    connect(ui->chartView, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(do_mouseMovePoint(QPoint))); // 鼠标移动事件
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_legendMarkerClicked()
{
    QLegendMarker *marker = qobject_cast<QLegendMarker *>(sender());
    marker->series()->setVisible(!marker->series()->isVisible()); // 序列的可见性
    marker->setVisible(true); // 图例标记总是可见的
    qreal alpha = 1.0;
    if (!marker->series()->isVisible())
        alpha = 0.5; // 设置为半透明表示序列不可见

    QBrush brush = marker->labelBrush();
    QColor color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setLabelBrush(brush); // 设置文字的brush

    brush = marker->brush();
    color = brush.color();
    color.setAlphaF(alpha);
    brush.setColor(color);
    marker->setBrush(brush); // 设置图例标记的brush
}

void MainWindow::do_mouseMovePoint(QPoint point)
{
    QPointF pt = chart->mapToValue(point); // 变换为图表的坐标
    QString str = QString::asprintf("Chart X=%.2f", pt.x(), pt.y());
    lab_chartXY->setText(str); // 状态栏上显示
}

void MainWindow::do_series_clicked(const QPointF &point)
{
    QString str = QString::asprintf("Clicked X=%.1f, Y=%.2f", point.x(), point.y());
    lab_clickXY->setText(str); // 状态栏显示
    QLineSeries *series = qobject_cast<QLineSeries *>(sender()); // 获取信号发射者
    int index = getIndexFromX(series, point.x()); // 获取数据点序列
    if (index < 0)
        return;
    bool isSelected = series->isPointSelected(index); // 数据点是否被选中
    series->setPointSelected(index, !isSelected); // 设置状态，选中或取消选中
}

void MainWindow::do_series_hovered(const QPointF &point, bool state)
{
    QString str = "Series X=, Y=";
    if (state)
        str = QString::asprintf("Hovered X=%.1f, Y=%.2f", point.x(), point.y());
    lab_hoverXY->setText(str); // 状态栏显示
    QLineSeries *series = qobject_cast<QLineSeries *>(sender()); // 获取信号发射者
    QPen pen = series->pen();
    if (state)
        pen.setColor(Qt::red); // 鼠标光标移入序列，序列变成红色
    else
        pen.setColor(Qt::black); // 鼠标光标移出序列，序列恢复为黑色
    series->setPen(pen);
}

void MainWindow::on_actDeselectAll_triggered()
{
    // 取消选点
    QXYSeries *series0 = qobject_cast<QXYSeries *>(chart->series().at(0));
    QXYSeries *series1 = qobject_cast<QXYSeries *>(chart->series().at(1));
    series0->deselectAllPoints(); // 取消选择所有数据点
    series1->deselectAllPoints();
}


void MainWindow::on_comboDragMode_currentIndexChanged(int index)
{
    // 设置拖动模式，dragMode，有3种模式：NoDrag、ScrollHandDrag、RubberBandDrag
    ui->chartView->setDragMode(QGraphicsView::DragMode(index));
}


void MainWindow::on_comboRubberBand_currentIndexChanged(int index)
{
    // 设置框选模式，rubberBand
    ui->chartView->setCustomZoomRect(index == 4); // 是否自定义模式
    // 必须有ClickThroughRubberBand，才能将clicked()信号传递给序列
    QFlags<QChartView::RubberBand> flags = QChartView::ClickThroughRubberBand;
    switch (index)
    {
    case 0:
        ui->chartView->setRubberBand(QChartView::NoRubberBand);
        break;
    case 1:
        flags |= QChartView::VerticalRubberBand; // 垂直方向选择
        break;
    case 2:
        flags |= QChartView::HorizontalRubberBand; // 水平方向选择
        break;
    case 3:
    case 4:
        flags |= QChartView::RectangleRubberBand; // 矩形框选
    }
    ui->chartView->setRubberBand(flags);
}

