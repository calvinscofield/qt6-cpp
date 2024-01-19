#include "tpendialog.h"
#include "ui_tpendialog.h"

#include <QColorDialog>
#include <QPen>

TPenDialog::TPenDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TPenDialog)
{
    ui->setupUi(this);
    // “线型”下拉列表框的选择项设置
    ui->comboPenStyle->clear();
    ui->comboPenStyle->addItem("NoPen"); // 添加字符串
    ui->comboPenStyle->addItem("SolidLine"); // 序号正好与Qt::PenStyle的枚举值对应
    ui->comboPenStyle->addItem("DashLine");
    ui->comboPenStyle->addItem("DotLine");
    ui->comboPenStyle->addItem("DashDotLine");
    ui->comboPenStyle->addItem("DashDotDotLine");
    ui->comboPenStyle->setCurrentIndex(1);
}

TPenDialog::~TPenDialog()
{
    delete ui;
}

void TPenDialog::setPen(QPen &pen)
{
    // 初始化pen，并刷新显示界面
    m_pen = pen;
    ui->spinWidth->setValue(pen.width()); // 线宽
    int i = static_cast<int>(pen.style()); // 枚举类型转换为整型
    ui->comboPenStyle->setCurrentIndex(i);

    QColor color = pen.color();
    ui->btnColor->setAutoFillBackground(true);
    QString str = QString::asprintf("background-color: rgb(%d, %d, %d);",
                                    color.red(), color.green(), color.blue());
    ui->btnColor->setStyleSheet(str);
}

QPen TPenDialog::getPen()
{
    // 获得设置的QPen对象
    m_pen.setStyle(Qt::PenStyle(ui->comboPenStyle->currentIndex())); // 线型
    m_pen.setWidth(ui->spinWidth->value()); // 线宽
    QColor color = ui->btnColor->palette().color(QPalette::Button);
    m_pen.setColor(color); // 线条颜色
    return m_pen;
}

QPen TPenDialog::getPen(QPen &iniPen, bool *ok)
{
    // 静态函数，获取QPen对象
    TPenDialog *dlg = new TPenDialog; // 创建对话框
    dlg->setPen(iniPen); // 设置初始化pen
    QPen pen;
    int ret = dlg->exec(); // 以模态方式显示对话框
    if (ret == QDialog::Accepted)
    {
        pen = dlg->getPen(); // 获取设置的结果
        *ok = true;
    }
    else
    {
        pen = iniPen;
        *ok = false;
    }
    delete dlg; // 删除对话框
    return pen;
}

void TPenDialog::on_btnColor_clicked()
{
    // 设置颜色
    QColor color = QColorDialog::getColor();
    if (color.isValid())
    {
        // 用样式表设置按钮的背景色
        QString str = QString::asprintf("background-color: rgb(%d, %d, %d);",
                                        color.red(), color.green(), color.blue());
        ui->btnColor->setStyleSheet(str);
    }
}

