#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractBarSeries>
#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QBarSet>
#include <QHorizontalBarSeries>
#include <QHorizontalStackedBarSeries>
#include <QLineSeries>
#include <QRandomGenerator>
#include <QStackedBarSeries>
#include <QValueAxis>

void MainWindow::generateData()
{
    // 数据初始化
    QStringList headerList;
    headerList << "姓名" << "数学" << "语文" << "英语" << "平均分";
    dataModel->setHorizontalHeaderLabels(headerList); // 设置表头文字
    QList<QStandardItem*> itemList; // 一行的item列表
    QStandardItem *item;

    for (int i = 0; i < studCount; i++)
    {
        itemList.clear();
        QString studName = QString::asprintf("学生%2d", i + 1);
        item = new QStandardItem(studName); // 创建item
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        itemList << item; // 添加到列表
        qreal avgScore = 0;
        for (int j = COL_MATH; j <= COL_ENGLISH; j++) // 数学，语文，英语
        {
            // 不包含最后一列
            qreal score = 50.0 + QRandomGenerator::global()->bounded(0, 50); // 随机数[0,50)
            avgScore += score;
            item = new QStandardItem(QString::asprintf("%.0f", score)); // 创建item
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            itemList << item; // 添加到列表
        }
        item = new QStandardItem(QString::asprintf("%.1f", avgScore / 3)); // 创建平均分item
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable)); // 平均分不允许编辑
        itemList << item; // 添加到列表
        dataModel->appendRow(itemList); // 添加一行
    }
}

void MainWindow::removeAllAxis(QChart *chart)
{
    // 删除一个chart的所有坐标轴
    QList<QAbstractAxis*> axisList = chart->axes(); // 获取坐标轴列表
    int count = axisList.count();
    for (int i = 0; i < count; i++)
    {
        QAbstractAxis *one = axisList.at(0);
        chart->removeAxis(one); // 从图表中移除坐标轴
        axisList.removeFirst(); // 从列表中移除坐标轴
        delete one; // 删除坐标轴对象，释放内存
    }
}

void MainWindow::iniBarChart()
{
    // 柱状图初始化
    QChart *chart = new QChart(); // 创建chart
    chart->setTitle("Barchart演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewBar->setChart(chart); // 为ChartView设置chart
    ui->chartViewBar->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::drawBarChart(bool isVertical)
{
    QChart *chart = ui->chartViewBar->chart(); // 获取ChartView关联的chart
    if (isVertical)
        chart->setTitle("Barchart演示");
    else
        chart->setTitle("Horizontal BarChart演示");
    chart->removeAllSeries(); // 删除所有序列
    removeAllAxis(chart); // 删除左右坐标轴

    // 创建一个QLineSeries序列用于显示平均分
    QLineSeries *seriesLine = new QLineSeries();
    seriesLine->setName("平均分");
    QPen pen(Qt::red);
    pen.setWidth(2);
    seriesLine->setPen(pen);
    QFont font = seriesLine->pointLabelsFont();
    font.setPointSize(12);
    font.setBold(true);
    seriesLine->setPointLabelsFont(font); // 显示数据点的标签
    seriesLine->setPointLabelsVisible(true);
    if (isVertical)
        seriesLine->setPointLabelsFormat("@yPoint"); // 标签显示的是Y坐标值
    else
        seriesLine->setPointLabelsFormat("@xPoint"); // 标签显示的是X坐标值

    // 创建3个QBarSet数据集，从数据模型获取数据
    QBarSet *setMath = new QBarSet("数学");
    QBarSet *setChinese = new QBarSet("语文");
    QBarSet *setEnglish = new QBarSet("英语");
    for (int i = 0; i < dataModel->rowCount(); i++)
    {
        setMath->append(dataModel->item(i, COL_MATH)->text().toInt()); // 数学
        setChinese->append(dataModel->item(i, COL_CHINESE)->text().toInt()); // 语文
        setEnglish->append(dataModel->item(i, COL_ENGLISH)->text().toInt()); // 英语
        if (isVertical)
            seriesLine->append(i, dataModel->item(i, COL_AVERAGE)->text().toDouble());
        else
            seriesLine->append(dataModel->item(i, COL_AVERAGE)->text().toDouble(), i);
    }
    // 创建一个柱状图序列QBarSeries，并添加3个数据集
    QAbstractBarSeries *seriesBar;
    if (isVertical)
        seriesBar =  new QBarSeries();
    else
        seriesBar = new QHorizontalBarSeries();
    seriesBar->setLabelsVisible(true); // 显示棒柱的标签
    seriesBar->setLabelsFormat("@value分"); // 棒柱标签格式
    seriesBar->append(setMath); // 添加数据集
    seriesBar->append(setChinese);
    seriesBar->append(setEnglish);
    connect(seriesBar, &QBarSeries::hovered, this, &MainWindow::do_barHovered);
    connect(seriesBar, &QBarSeries::clicked, this, &MainWindow::do_barClicked);
    chart->addSeries(seriesBar); // 添加柱状图序列
    chart->addSeries(seriesLine); // 添加折线图序列

    // QBarCategoryAxis坐标轴
    QStringList categories;
    for (int i = 0; i < dataModel->rowCount(); i++)
        categories << dataModel->item(i, COL_NAME)->text();
    QBarCategoryAxis *axisStud = new QBarCategoryAxis(); // 用于柱状图的坐标轴
    axisStud->append(categories); // 添加字符串列表作为坐标值
    // 坐标范围
    axisStud->setRange(categories.at(0), categories.at(categories.count() - 1));

    // QValueAxis坐标轴
    QValueAxis *axisValue = new QValueAxis();
    axisValue->setRange(1, 100);
    axisValue->setTitleText("分数");
    axisValue->setTickCount(6);
    axisValue->setLabelFormat("%.0f"); // 标签格式
    axisValue->applyNiceNumbers();

    // 为chart和序列添加坐标轴
    if (isVertical)
    {
        chart->addAxis(axisStud, Qt::AlignBottom);
        chart->addAxis(axisValue, Qt::AlignLeft);
    }
    else
    {
        chart->addAxis(axisStud, Qt::AlignLeft);
        chart->addAxis(axisValue, Qt::AlignBottom);
    }
    seriesBar->attachAxis(axisStud);
    seriesBar->attachAxis(axisValue);
    seriesLine->attachAxis(axisStud);
    seriesLine->attachAxes(axisStud);
    chart->legend()->setAlignment(Qt::AlignBottom); // 图例显示在下方
}

void MainWindow::iniStackedBar()
{
    // 堆叠柱状图初始化
    QChart *chart = new QChart();
    chart->setTitle("StackedBar演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewStackedBar->setChart(chart);
    ui->chartViewStackedBar->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::drawStackedBar(bool isVertical)
{
    QChart *chart = ui->chartViewStackedBar->chart(); // 获取QChart对象
    if (isVertical)
        chart->setTitle("StackedBar演示");
    else
        chart->setTitle("Horizontal StackedBar演示");
    chart->removeAllSeries(); // 移除所有序列
    removeAllAxis(chart); // 移除所有坐标轴

    // 创建3门课的数据集，从数据模型获取数据
    QBarSet *setMath = new QBarSet("数学");
    QBarSet *setChinese = new QBarSet("语文");
    QBarSet *setEnglish = new QBarSet("英语");
    for (int i = 0; i < dataModel->rowCount(); i++)
    {
        setMath->append(dataModel->item(i, COL_MATH)->text().toInt());
        setChinese->append(dataModel->item(i, COL_CHINESE)->text().toInt());
        setEnglish->append(dataModel->item(i, COL_ENGLISH)->text().toInt());
    }
    // 创建序列，添加数据集
    QAbstractBarSeries *seriesBar;
    if (isVertical)
        seriesBar =  new QStackedBarSeries();
    else
        seriesBar = new QHorizontalStackedBarSeries();
    seriesBar->append(setMath); // 添加数据集
    seriesBar->append(setChinese);
    seriesBar->append(setEnglish);
    seriesBar->setLabelsVisible(true); // 显示每小段的标签
    connect(seriesBar, &QBarSeries::hovered, this, &MainWindow::do_barHovered);
    connect(seriesBar, &QBarSeries::clicked, this, &MainWindow::do_barClicked);
    chart->addSeries(seriesBar); // 添加序列到图表中

    // 创建QBarCategoryAxis坐标轴
    QStringList categories;
    for (int i = 0; i < dataModel->rowCount(); i++)
        categories << dataModel->item(i, COL_NAME)->text();
    QBarCategoryAxis *axisStud = new QBarCategoryAxis(); // 类别坐标轴
    axisStud->append(categories);
    axisStud->setRange(categories.at(0), categories.at(categories.count() - 1));

    // 创建QValueAxis坐标轴
    QValueAxis *axisValue = new QValueAxis(); // 数值坐标轴
    axisValue->setRange(0, 300);
    axisValue->setTitleText("总分");
    axisValue->setTickCount(7);
    axisValue->setLabelFormat("%.0f"); // 标签格式
    // 为chart和序列添加坐标轴
    if (isVertical)
    {
        chart->addAxis(axisStud, Qt::AlignBottom);
        chart->addAxis(axisValue, Qt::AlignLeft);
    }
    else
    {
        chart->addAxis(axisStud, Qt::AlignLeft);
        chart->addAxis(axisValue, Qt::AlignBottom);
    }
    seriesBar->attachAxis(axisStud);
    seriesBar->attachAxis(axisValue);
    chart->legend()->setAlignment(Qt::AlignRight);
}

void MainWindow::iniPercentBar()
{
    // 百分比柱状图初始化
    QChart *chart = new QChart();
    chart->setTitle("PercentBar演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewPercentBar->setChart(chart);
    ui->chartViewPercentBar->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::iniPieChart()
{
    // 饼图初始化
    QChart *chart = new QChart();
    chart->setTitle("Piechart演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewPie->setChart(chart);
    ui->chartViewPie->setRenderHint(QPainter::Antialiasing);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setAlternatingRowColors(true);
    ui->treeWidget->setAlternatingRowColors(true);
    studCount = ui->spinCount->value(); // 学生人数
    dataModel = new QStandardItemModel(this); // 数据模型
    generateData(); // 初始化数据
    countData(); // 数据统计
    // 数据模型的itemChanged()信号与自定义的槽函数关联，用于在修改数据后自动计算平均分
    connect(dataModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(do_calcuAverage(QStandardItem*)));
    ui->tableView->setModel(dataModel); // 设置数据模型
    iniBarChart(); // 柱状图初始化
    iniStackedBar(); // 堆叠柱状图初始化
    iniPercentBar(); // 百分比柱状图初始化
    iniPieChart(); // 饼图初始化
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_calcuAverage(QStandardItem *item)
{
    // 自动计算平均分
    if ((item->column() < COL_MATH) || (item->column() > COL_ENGLISH))
        return; // 如果被修改的item不是数学、语文、英语数据就退出
    int rowNo = item->row();
    qreal avg = 0;
    QStandardItem *aItem;
    for (int i = COL_MATH; i <= COL_ENGLISH; i++)
    {
        aItem = dataModel->item(rowNo, i);
        avg += aItem->text().toDouble();
    }
    avg = avg / 3; // 计算平均分
    aItem = dataModel->item(rowNo, COL_AVERAGE); // 获取平均分数据的item
    aItem->setText(QString::asprintf("%.1f", avg)); // 更新平均分数据
}

void MainWindow::do_barHovered(bool status, int index, QBarSet *barset)
{
    QString str = "hovered barSet=" + barset->label();
    if (status)
        str += QString::asprintf(", index=%d, value=%.2f", index, barset->at(index));
    else
        str = "";
    ui->statusBar->showMessage(str);
}

void MainWindow::do_barClicked(int index, QBarSet *barset)
{
    QString str = "clicked barSet=" + barset->label();
    str += QString::asprintf(", index=%d, count=%d", index, barset->count());
    ui->statusBar->showMessage(str);
}

void MainWindow::on_btnBuildBarChart_clicked()
{
    drawBarChart(true); // 绘制柱状图
}


void MainWindow::on_btnBuildBarChartH_clicked()
{
    drawBarChart(false); // 绘制水平柱状图
}


void MainWindow::on_btnBuildStackedBar_clicked()
{
    drawStackedBar(true); // 绘制堆叠柱状图
}


void MainWindow::on_btnBuildStackedBarH_clicked()
{
    drawStackedBar(false); // 绘制水平堆叠柱状图
}

