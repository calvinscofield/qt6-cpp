#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化一个字符串列表的内容
    m_strList << "北京" << "上海" << "天津" << "河北" << "山东" << "四川" << "重庆" << "广东" << "河南";
    m_model = new QStringListModel(this); // 创建数据模型
    m_model->setStringList(m_strList); // 初始化数据
    ui->listView->setModel(m_model); // 设置数据模型
    ui->chkEditable->setChecked(true);
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnIniList_clicked()
{
    // “恢复列表”按钮
    m_model->setStringList(m_strList); // 重新载入
}


void MainWindow::on_btnListClear_clicked()
{
    // “清除列表”按钮
    m_model->removeRows(0, m_model->rowCount()); // 清除数据模型的所有项
}



void MainWindow::on_btnListAppend_clicked()
{
    // “添加项”按钮
    m_model->insertRow(m_model->rowCount()); // 在末尾插入一个项
    QModelIndex index = m_model->index(m_model->rowCount() - 1, 0); // 获取刚插入项的模型索引
    m_model->setData(index, "new item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}


void MainWindow::on_btnListInsert_clicked()
{
    // “插入项”按钮
    QModelIndex index = ui->listView->currentIndex(); // 当前项的模型索引
    m_model->insertRow(index.row());
    m_model->setData(index, "inserted item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}


void MainWindow::on_btnListDelete_clicked()
{
    // “删除项”按钮
    QModelIndex index = ui->listView->currentIndex(); // 获取当前项的模型索引
    m_model->removeRow(index.row());
}


void MainWindow::on_btnListMoveUp_clicked()
{
    // “上移”按钮
    int curRow = ui->listView->currentIndex().row(); // 当前行的行号
    QModelIndex index = QModelIndex();
    m_model->moveRow(index, curRow, index, curRow - 1);
}


void MainWindow::on_btnListMoveDown_clicked()
{
    // “下移”按钮
    int curRow = ui->listView->currentIndex().row(); // 当前行的行号
    QModelIndex index = QModelIndex();
    m_model->moveRow(index, curRow, index, curRow + 2);
}


void MainWindow::on_btnListSort_clicked(bool checked)
{
    // “排序”按钮
    if (checked)
        m_model->sort(0, Qt::AscendingOrder); // 升序
    else
        m_model->sort(0, Qt::DescendingOrder); // 降序
}


void MainWindow::on_chkEditable_clicked(bool checked)
{
    // “允许编辑”复选框
    if (checked) // 可编辑
        ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    else
        ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    // 组件listView的clicked()信号的槽函数
    QString str1 = QString::asprintf("模型索引：row = %d，column = %d；", index.row(), index.column());
    QVariant var = m_model->data(index, Qt::DisplayRole); // 获取模型数据
    QString str2 = var.toString();
    ui->statusbar->showMessage(str1 + str2);
}


void MainWindow::on_btnTextImport_clicked()
{
    // “显示数据模型的StringList”按钮
    QStringList tmpList = m_model->stringList();
    ui->plainTextEdit->clear();
    for (int i = 0; i < tmpList.size(); i++)
        ui->plainTextEdit->appendPlainText(tmpList.at(i));
}


void MainWindow::on_btnTextClear_clicked()
{
    // 清除plainTextEdit的文本
    ui->plainTextEdit->clear();
}

