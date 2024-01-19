#include "mainwindow.h"
#include "tpendialog.h"
#include "ui_mainwindow.h"

void MainWindow::createChart()
{
    // 创建图表
    chart = new QChart();
    chart->setTitle(tr("简单函数曲线"));
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    QLineSeries *series0 = new QLineSeries();
    QLineSeries *series1 = new QLineSeries();
    series0->setName("Sin曲线");
    series1->setName("Cos曲线");
    curSeries = series0; // 当前序列

    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    series0->setPen(pen); // 序列series0的线条设置

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    series1->setPen(pen); // 序列series1的线条设置
    chart->addSeries(series0); // 将序列添加到图表中
    chart->addSeries(series1);

    QValueAxis *axisX = new QValueAxis;
    curAxis = axisX; // 当前坐标轴
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%.1f"); // 标签格式
    axisX->setTickCount(11); // 主刻度个数
    axisX->setMinorTickCount(2); // 次刻度个数
    axisX->setTitleText("time(secs)"); // 轴标题

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-2, 2);
    axisY->setLabelFormat("%.2f"); // 标签格式
    axisY->setTickCount(5);
    axisY->setMinorTickCount(2);
    axisY->setTitleText("value");

    // 为chart和序列设置坐标轴
    chart->addAxis(axisX, Qt::AlignBottom); // 将坐标轴添加到图表中，并指定方向
    chart->addAxis(axisY, Qt::AlignLeft);
    series0->attachAxis(axisX); // 序列series0，附加坐标轴
    series0->attachAxis(axisY);
    series1->attachAxis(axisX); // 序列series1，附加坐标轴
    series1->attachAxis(axisY);
}

void MainWindow::prepareData()
{
    // 为序列生成数据
    QLineSeries *series0 = static_cast<QLineSeries *>(chart->series().at(0));
    QLineSeries *series1 = static_cast<QLineSeries *>(chart->series().at(1));
    series0->clear(); // 清除数据
    series1->clear();
    qreal t = 0, y1, y2, intv = 0.1;
    int cnt = 100;
    for (int i = 0; i < cnt; i++)
    {
        int rd = QRandomGenerator::global()->bounded(-5, 6); // 随机整数，[-5,5]
        y1 = qSin(t) + rd / 50.0;
        series0->append(t, y1); // 序列添加数据点
        rd = QRandomGenerator::global()->bounded(-5, 6); // 随机整数，[-5,5]
        y2 = qCos(t) + rd / 50.0;
        series1->append(t, y2); // 序列添加数据点
        t += intv;
    }
}

void MainWindow::updateFromChart()
{
    QChart *curChart = ui->chartView->chart(); // 获取视图组件关联的chart
    ui->editTitle->setText(curChart->title()); // 图表标题
    QMargins mg = curChart->margins(); // 图表的边距
    ui->spinMarginLeft->setValue(mg.left());
    ui->spinMarginRight->setValue(mg.right());
    ui->spinMarginTop->setValue(mg.top());
    ui->spinMarginBottom->setValue(mg.bottom());
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createChart(); // 创建图表
    prepareData(); // 为序列生成数据
    updateFromChart(); // 从图表获取属性值，刷新界面显示内容
    this->setCentralWidget(ui->splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}
////=======1.1 图表标题========
void MainWindow::on_btnTitleSetText_clicked()
{
    // 设置图表标题文字
    QString str = ui->editTitle->text();
    chart->setTitle(str);
}


void MainWindow::on_btnTitleColor_clicked()
{
    // 设置图表标题文字颜色
    QColor color = chart->titleBrush().color();
    color = QColorDialog::getColor(color);
    if (color.isValid())
        chart->setTitleBrush(QBrush(color));
}



void MainWindow::on_btnTitleFont_clicked()
{
    // 设置图表标题文字的字体
    QFont font = chart->titleFont();
    bool ok = false;
    font = QFontDialog::getFont(&ok, font);
    if (ok)
        chart->setTitleFont(font);
}

////=======1.2 图例========
void MainWindow::on_groupBox_Legend_clicked(bool checked)
{
    // 图例是否可见
    chart->legend()->setVisible(checked);
}


void MainWindow::on_radioButton_clicked()
{
    // 图例的位置，上方
    chart->legend()->setAlignment(Qt::AlignTop);
}


void MainWindow::on_btnLegendlabelColor_clicked()
{
    // 图例的文字颜色
    QColor color = chart->legend()->labelColor();
    color = QColorDialog::getColor(color);
    if (color.isValid())
        chart->legend()->setLabelColor(color);
}


void MainWindow::on_btnLegendFont_clicked()
{
    // 图例的字体设置
    QFont font = chart->legend()->font();
    bool ok = false;
    font = QFontDialog::getFont(&ok, font);
    if (ok)
        chart->legend()->setFont(font);
}

////=======1.3 边距========
void MainWindow::on_btnSetMargin_clicked()
{
    // 设置图表的4个边距值
    QMargins mgs;
    mgs.setLeft(ui->spinMarginLeft->value());
    mgs.setRight(ui->spinMarginRight->value());
    mgs.setTop(ui->spinMarginTop->value());
    mgs.setBottom(ui->spinMarginBottom->value());
    chart->setMargins(mgs);
}

////=======1.4 动画效果和主题========
void MainWindow::on_comboAnimation_currentIndexChanged(int index)
{
    // 动画效果
    chart->setAnimationOptions(QChart::AnimationOptions(index));
}


void MainWindow::on_comboTheme_currentIndexChanged(int index)
{
    // 图表的主题
    chart->setTheme(QChart::ChartTheme(index));
}


void MainWindow::on_radioSeries0_clicked()
{
    // 选择操作序列，Sin曲线
    if (ui->radioSeries0->isChecked())
        curSeries = static_cast<QLineSeries *>(chart->series().at(0));
    else
        curSeries = static_cast<QLineSeries *>(chart->series().at(1));
    // 获取序列的属性值，并显示到界面上
    ui->editSeriesName->setText(curSeries->name()); // 序列名称
    ui->groupBox_Series->setChecked(curSeries->isVisible()); // 序列可见
    ui->groupBox_Points->setChecked(curSeries->pointsVisible()); // 数据点可见
    ui->chkkBoxUseOpenGL->setChecked(curSeries->useOpenGL()); // 使用OpenGL
    ui->sliderOpacity->setValue(curSeries->opacity() * 10); // 透明度
    ui->groupBox_PointLabel->setChecked(curSeries->pointLabelsVisible()); // 数据点标签可见
}

void MainWindow::on_radioSeries1_clicked()
{
    // 选择操作序列，Cos曲线
    on_radioSeries0_clicked();
}

////=======2.1 序列曲线设置========
void MainWindow::on_groupBox_Series_clicked(bool checked)
{
    // 设置序列是否可见
    this->curSeries->setVisible(checked);
}


void MainWindow::on_btnSeriesColor_clicked()
{
    // 设置序列的曲线颜色
    QColor color = curSeries->color();
    color = QColorDialog::getColor(color);
    if (color.isValid())
        curSeries->setColor(color);
}


void MainWindow::on_btnSeriesPen_clicked()
{
    // 设置序列线条的pen属性
    QPen pen = curSeries->pen();
    bool ok = false;
    pen = TPenDialog::getPen(pen, &ok); // 使用静态函数设置pen属性
    if (ok)
        curSeries->setPen(pen);
}


void MainWindow::on_chkkBoxUseOpenGL_clicked(bool checked)
{
    // 设置是否使用OpenGL加速
    curSeries->setUseOpenGL(checked);
}

////=======2.2 数据点========
void MainWindow::on_groupBox_Points_clicked(bool checked)
{
    // 序列的数据点是否可见
    curSeries->setPointsVisible(checked);
}


void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    // 设置数据点大小
    curSeries->setMarkerSize(arg1);
}


void MainWindow::on_btnPointColor_clicked()
{
    // 设置数据点填充颜色
    QColor color = QColorDialog::getColor();
    if (color.isValid())
        curSeries->setBrush(QBrush(color));
}

////=======2.3 数据点标签========
void MainWindow::on_groupBox_PointLabel_clicked(bool checked)
{
    // 是否显示数据点标签
    curSeries->setPointLabelsVisible(checked);
}


void MainWindow::on_radioSeriesLabFormat0_clicked()
{
    // 序列数据点标签的显示格式
    curSeries->setPointLabelsFormat("@yPoint");
}


void MainWindow::on_radioSeriesLabFormat1_clicked()
{
    // 序列数据点标签的显示格式
    curSeries->setPointLabelsFormat("(@xPoint,@yPoint)");
}


void MainWindow::on_radioAxisX_clicked()
{
    // 获取当前坐标轴，X轴
    if (ui->radioAxisX->isChecked()) // 获取X轴对象
        curAxis = static_cast<QValueAxis *>(chart->axes(Qt::Horizontal).at(0));
    else // 获取Y轴对象
        curAxis = static_cast<QValueAxis *>(chart->axes(Qt::Vertical).at(0));
    // 获取坐标轴的各种属性值，并显示到界面上
    ui->groupBox_Axis->setChecked(curAxis->isVisible()); // 坐标轴可见
    ui->chkBoxAxisReverse->setChecked(curAxis->isReverse()); // 坐标轴反向
    ui->spinAxisMin->setValue(curAxis->min()); // 坐标轴最小值
    ui->spinAxisMax->setValue(curAxis->max()); // 坐标轴最大值
    // 轴标题和轴刻度标签
    ui->groupBox_AxisTitle->setChecked(curAxis->isTitleVisible()); // 轴标题可见
    ui->editAxisTitle->setText(curAxis->titleText()); // 轴标题文字
    ui->groupBox_AxisLabel->setChecked(curAxis->labelsVisible()); // 轴刻度标签可见
    ui->editAxisLabelFormat->setText(curAxis->labelFormat()); // 标签格式
    // 主刻度和次刻度
    ui->groupBox_Ticks->setChecked(curAxis->isLineVisible()); // 主刻度线可见
    if (curAxis->tickType() == QValueAxis::TicksFixed) // 主刻度类型
        ui->radioTick_Fixed->setChecked(true); // 固定刻度
    else
        ui->radioTick_Dynamic->setChecked(true); // 动态刻度
    ui->spinTick_Anchor->setValue(curAxis->tickAnchor()); // 动态刻度起始值
    ui->spinTick_Intv->setValue(curAxis->tickInterval()); // 动态刻度间隔值
    ui->spinTick_Count->setValue(curAxis->tickCount()); // 主刻度个数
    // 主网格线和次网格线
    ui->groupBox_GridLine->setChecked(curAxis->isGridLineVisible()); // 主网格线可见
    ui->groupBox_MinorGrid->setChecked(curAxis->isMinorGridLineVisible()); // 次网格线可见
    ui->spinMinorTickCount->setValue(curAxis->minorTickCount()); // 次刻度个数
}


void MainWindow::on_radioAxisY_clicked()
{
    // 获取当前坐标轴，Y轴
    on_radioAxisX_clicked();
}

////=======3.1 坐标轴可见性和范围========
void MainWindow::on_groupBox_Axis_clicked(bool checked)
{
    // 坐标轴可见性
    curAxis->setVisible(checked);
}


void MainWindow::on_btnSetAxisRange_clicked()
{
    // 设置坐标轴的坐标范围
    curAxis->setRange(ui->spinAxisMin->value(), ui->spinAxisMax->value());
}

////=======3.2 轴标题========
void MainWindow::on_groupBox_AxisTitle_clicked(bool checked)
{
    // 轴标题的可见性
    curAxis->setTitleVisible(checked);
}


void MainWindow::on_btnAxisSetTitle_clicked()
{
    // 设置坐标轴的标题文字
    curAxis->setTitleText(ui->editAxisTitle->text());
}

////=======3.3 轴刻度标签========
void MainWindow::on_groupBox_AxisLabel_clicked(bool checked)
{
    // 轴刻度标签可见性
    curAxis->setLabelsVisible(checked);
}


void MainWindow::on_btnAxisLabelFormat_clicked()
{
    // 设置轴刻度标签的文字格式
    curAxis->setLabelFormat(ui->editAxisLabelFormat->text()); // 如"%.2f"
}

////=======3.4 轴线和主刻度========
void MainWindow::on_groupBox_Ticks_clicked(bool checked)
{
    // 轴线和主刻度的可见性
    curAxis->setLineVisible(checked);
}


void MainWindow::on_radioTick_Fixed_clicked()
{
    // “固定刻度”d单选按钮复选状态变化，更新组件的enabled属性
    if (checked)
    {
        ui->spinTick_Count->setEnabled(true);
        ui->spinTick_Anchor->setEnabled(false);
        ui->spinTick_Intv->setEnabled(false);
    }
    else
    {
        ui->spinTick_Count->setEnabled(false);
        ui->spinTick_Anchor->setEnabled(true);
        ui->spinTick_Intv->setEnabled(true);
    }
}


void MainWindow::on_spinTick_Count_valueChanged(int arg1)
{
    // 设置主刻度个数，固定刻度模式下有效
    curAxis->setTickCount(arg1);
}


void MainWindow::on_spinTick_Anchor_valueChanged(double arg1)
{
    // 设置动态刻度的起点值
    curAxis->setTickAnchor(arg1);
}


void MainWindow::on_spinTick_Intv_valueChanged(double arg1)
{
    // 设置动态刻度的间隔值
    curAxis->setTickInterval(arg1);
}

////=======3.5 主网格线========
void MainWindow::on_groupBox_GridLine_clicked(bool checked)
{
    // 主网格线可见性
    curAxis->setGridLineVisible(checked);
}


void MainWindow::on_btnGridLinePen_clicked()
{
    // 主网格线的线条pen设置
    QPen pen = curAxis->gridLinePen();
    bool ok = false;
    pen = TPenDialog::getPen(pen, &ok);
    if (ok)
        curAxis->setGridLinePen(pen);
}

////=======3.6 次网格线========
void MainWindow::on_groupBox_MinorGrid_clicked(bool checked)
{
    // 次网格线可见性
    curAxis->setMinorGridLineVisible(checked);
}


void MainWindow::on_spinMinorTickCount_valueChanged(int arg1)
{
    // 次刻度个数
    curAxis->setMinorTickCount(arg1);
}

