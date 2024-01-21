#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QSplitter>

void MainWindow::iniGraph3D()
{
    graph3D = new Q3DScatter();
    graphContainer = QWidget::createWindowContainer(graph3D, this);
    QScatterDataProxy *proxy = new QScatterDataProxy(); // 创建数据代理
    series = new QScatter3DSeries(proxy); // 创建序列
    series->setItemLabelFormat("(@xLabel, @zLabel, @yLabel)"); // 散点标签的格式(x,z,y)
    series->setMeshSmooth(true);
    series->setBaseColor(Qt::yellow);
    graph3D->addSeries(series);

    // 使用内置的坐标轴
    graph3D->axisX()->setTitle("axis X");
    graph3D->axisX()->setLabelFormat("%.2f"); // 设置轴标签格式
    graph3D->axisX()->setTitleVisible(true);
    graph3D->axisY()->setTitle("axis Y");
    graph3D->axisY()->setLabelFormat("%.2f");
    graph3D->axisY()->setTitleVisible(true);
    graph3D->axisZ()->setTitle("axis Z");
    graph3D->axisZ()->setLabelFormat("%.2f");
    graph3D->axisZ()->setTitleVisible(true);
    graph3D->activeTheme()->setLabelBackgroundEnabled(false); // 不显示轴标签背景
    series->setMesh(QAbstract3DSeries::MeshSphere); // 设置散点形状
    series->setItemSize(0.2); // 设置散点大小，取值范围是0~1，默认值是0

    int N = 41;
    int itemCount = N * N; // 数据点总数
    QScatterDataArray *dataArray = new QScatterDataArray(); // 散点对象数组
    dataArray->resize(itemCount); // 设置数组大小
    QScatterDataItem *ptrToDataArray = &dataArray->first(); // 首地址
    // “墨西哥草帽”图，-10:0.5:10, N=41
    float x, y, z;
    x = -10;
    for (int i = 1; i <= N; i++)
    {
        y = -10;
        for (int j = 1; j <= N; j++)
        {
            z = qSqrt(x * x + y * y);
            if (z != 0)
                z = 10 * qSin(z) / z;
            else
                z = 10;
            ptrToDataArray->setPosition(QVector3D(x, z, y)); // 设置坐标
            ptrToDataArray++; // 指向下一个元素
            y += 0.5;
        }
        x += 0.5;
    }
    series->dataProxy()->resetArray(dataArray); // 设置数据数组
    connect(series, &QScatter3DSeries::selectedItemChanged, this, &MainWindow::do_itemSelected);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sliderZoom->setRange(10, 500); // 设置数据范围
    ui->sliderH->setRange(-180, 180); // 水平旋转角度范围
    ui->sliderV->setRange(-180, 180); // 垂直旋转角度范围
    iniGraph3D(); // 创建三维柱状图，并会创建一个容器对象graphContainer
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->frameSetup); // 左侧控制面板
    splitter->addWidget(graphContainer); // 右侧三维图
    this->setCentralWidget(splitter); // 设置主窗口中心组件
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_itemSelected(int index)
{
    ui->actPoint_ChangeValue->setEnabled(index >= 0);
    ui->actData_Delete->setEnabled(index >= 0);
    if (index >= 0) // index是散点数据序号
    {
        QScatterDataItem item = *(series->dataProxy()->itemAt(index)); // 当前选中的散点
        QString str = QString::asprintf("选中点的坐标(x,z,y)=(%.2f,%.2f,%.2f)", item.x(), item.z(), item.y());
        ui->statusBar->showMessage(str);
    }
}

void MainWindow::on_chkBoxShadow_clicked(bool checked)
{
    // 显示阴影
    if (checked)
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
    // 中等品质阴影
    else
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityNone); // 无阴影
}


void MainWindow::on_spinItemSize_valueChanged(double arg1)
{
    // 散点大小，SpinBox
    series->setItemSize(arg1); // 设置散点大小，取值范围是0~1，默认值是0
}


void MainWindow::on_actPoint_ChangeValue_triggered()
{
    // 修改当前选中散点的坐标
    int index = series->selectedItem(); // 当前选中散点的序号
    if (index < 0)
        return;

    QScatterDataItem item = *(series->dataProxy()->itemAt(index));
    QString coord = QString::asprintf("%.2f, %.2f, %.2f", item.x(), item.z(), item.y());
    bool ok = false;
    QString newText = QInputDialog::getText(this, "修改点坐标", "按格式输入点的坐标(x,z,y)", QLineEdit::Normal, coord, &ok);
    if (!ok)
        return;
    newText = newText.simplified(); // 去除前后和中间的空格
    QStringList xzy = newText.split(QLatin1Char(','), Qt::SkipEmptyParts); // 用逗号分割
    if (xzy.size() != 3)
    {
        QMessageBox::critical(this, "错误", "输入坐标数据格式错误");
        return;
    }

    item.setX(xzy[0].toFloat()); // 设置散点坐标
    item.setZ(xzy[1].toFloat());
    item.setY(xzy[2].toFloat());
    series->dataProxy()->setItem(index, item); // 重新设置散点数据项
}


void MainWindow::on_actData_Add_triggered()
{
    // 添加随机点
    int x = QRandomGenerator::global()->bounded(-10, 10);
    int z = QRandomGenerator::global()->bounded(-10, 10);
    int y = QRandomGenerator::global()->bounded(5, 10);
    QScatterDataItem item;
    item.setX(x);
    item.setY(y);
    item.setZ(z);
    series->dataProxy()->addItem(item);
}


void MainWindow::on_actData_Delete_triggered()
{
    // 删除当前点
    int index = series->selectedItem(); // 当前选中散点的序号
    if (index < 0)
        return;
    int removeCount = 1; // 删除点个数
    series->dataProxy()->removeItems(index, removeCount);
}

