#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSplitter>

void MainWindow::iniGraph3D()
{
    graph3D = new Q3DSurface();
    graphContainer = QWidget::createWindowContainer(graph3D, this);
    // 创建坐标轴
    QValue3DAxis *axisX = new QValue3DAxis; // X，东西
    axisX->setTitle("AxisX：西--东");
    axisX->setTitleVisible(true);
    axisX->setLabelFormat("%.1f 米");
    axisX->setRange(-5000, 5000);
    graph3D->setAxisX(axisX);

    QValue3DAxis *axisY = new QValue3DAxis; // Y，高度
    axisY->setTitle("AxisY：高度");
    axisY->setTitleVisible(true);
    axisY->setAutoAdjustRange(true);
    graph3D->setAxisY(axisY);

    QValue3DAxis *axisZ = new QValue3DAxis; // Z，南北
    axisZ->setTitle("AxisZ：南--北");
    axisZ->setLabelFormat("%.1f 米");
    axisZ->setTitleVisible(true);
    axisZ->setRange(-5000, 5000);
    graph3D->setAxisZ(axisZ);

    // 创建数据代理
    QImage heightMapImage(":/map/mountain.png");
    QHeightMapSurfaceDataProxy *proxy = new QHeightMapSurfaceDataProxy(heightMapImage);
    proxy->setValueRanges(-5000, 5000, -5000, 5000);
    series = new QSurface3DSeries(proxy);
    series->setItemLabelFormat("(@xLabel,@zLabel,@yLabel)");
    series->setFlatShadingEnabled(false);
    series->setMeshSmooth(true);
    series->setDrawMode(QSurface3DSeries::DrawSurface);
    graph3D->addSeries(series);
    connect(series, &QSurface3DSeries::selectedPointChanged, this, &MainWindow::do_pointSelected);
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

void MainWindow::do_pointSelected(const QPoint &position)
{
    if ((position.x() < 0) || (position.y() < 0))
        return;
    QSurfaceDataItem item = *(series->dataProxy()->itemAt(position)); // 获取点数据项
    QString str = QString::asprintf("选中点的坐标(x,z,y)=(%.2f,%.2f,%.2f)", item.x(), item.z(), item.y());
    ui->statusBar->showMessage(str);
}

void MainWindow::on_actPoint_Modify_triggered()
{
    // 修改点坐标
    QPoint point = series->selectedPoint();
    if ((point.x() < 0) || point.y() < 0)
        return;

    QSurfaceDataItem item = *(series->dataProxy()->itemAt(point)); // 获取点数据项
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
    item.setX(xzy[0].toFloat());
    item.setZ(xzy[1].toFloat());
    item.setY(xzy[2].toFloat());
    series->dataProxy()->setItem(point, item);
}


void MainWindow::on_actPoint_DeleteRow_triggered()
{
    // 删除行
    QPoint point = series->selectedPoint();
    if ((point.x() < 0) || (point.y() < 0))
        return;
    int removeCount = 1; // 删除行的数据
    series->dataProxy()->removeRows(point.x(), removeCount);
}


void MainWindow::on_comboDrawMode_currentIndexChanged(int index)
{
    // 曲面样式
    if (index == 0)
        series->setDrawMode(QSurface3DSeries::DrawWireframe);
    else if (index == 1)
        series->setDrawMode(QSurface3DSeries::DrawSurface);
    else
        series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
}


void MainWindow::on_chkBoxFlatShading_clicked(bool checked)
{
    series->setFlatShadingEnabled(checked);
}


void MainWindow::on_actSurf_Color_triggered()
{
    // 设置曲面颜色
    QColor color = series->baseColor();
    color = QColorDialog::getColor(color);
    if (color.isValid())
    {
        series->setBaseColor(color);
        series->setColorStyle(Q3DTheme::ColorStyleUniform); // 单一颜色
    }
}


void MainWindow::on_actSurf_GradColor1_triggered()
{
    // 渐变色一
    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::blue);
    gr.setColorAt(1.0, Qt::yellow);
    series->setBaseGradient(gr);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient); // 渐变色
}


void MainWindow::on_actSurf_GradColor2_triggered()
{
    // 渐变色二
    QLinearGradient grGtoR;
    grGtoR.setColorAt(0.0, Qt::cyan);
    grGtoR.setColorAt(1.0, Qt::red);
    series->setBaseGradient(grGtoR);
    series->setColorStyle(Q3DTheme::ColorStyleRangeGradient); // 渐变色
}

