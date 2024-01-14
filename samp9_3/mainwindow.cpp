#include "mainwindow.h"
#include "tdialogdata.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

void MainWindow::selectData()
{
    qryModel = new QSqlQueryModel(this);
    selModel = new QItemSelectionModel(qryModel, this);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(selModel);
    qryModel->setQuery("SELECT empNo, Name, Gender, Birthday, Province, Department,"
                       "Salary FROM employee ORDER BY empNo");
    if (qryModel->lastError().isValid())
    {
        QMessageBox::information(this, "错误", "数据表查询错误，错误信息\n" + qryModel->lastError().text());
        return;
    }

    QSqlRecord rec = qryModel->record(); // 获取空记录，用于获取字段序号
    // 设置字段显示标题
    qryModel->setHeaderData(rec.indexOf("empNo"), Qt::Horizontal, "工号");
    qryModel->setHeaderData(rec.indexOf("Name"), Qt::Horizontal, "姓名");
    qryModel->setHeaderData(rec.indexOf("Gender"), Qt::Horizontal, "性别");
    qryModel->setHeaderData(rec.indexOf("Birthday"), Qt::Horizontal, "出生日期");
    qryModel->setHeaderData(rec.indexOf("Province"), Qt::Horizontal, "省份");
    qryModel->setHeaderData(rec.indexOf("Department"), Qt::Horizontal, "部门");
    qryModel->setHeaderData(rec.indexOf("Salary"), Qt::Horizontal, "工资");

    ui->actOpenDB->setEnabled(false);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);
    ui->actRecEdit->setEnabled(true);
    ui->actScan->setEnabled(true);
}

void MainWindow::updateRecord(int recNo)
{
    // 更新一条记录
    QSqlRecord curRec = qryModel->record(recNo); // 获取数据模型的一条记录
    int empNo = curRec.value("EmpNo").toInt(); // 获取EmpNo字段的值
    QSqlQuery query;
    query.prepare("SELECT * FROM employee WHERE EmpNo = :ID");
    query.bindValue(":ID", empNo);
    query.exec();
    query.first();
    if (!query.isValid()) // 无有效记录
        return;

    curRec = query.record(); // 获取当前记录
    TDialogData *dataDialog = new TDialogData(this); // 创建对话框
    Qt::WindowFlags flags = dataDialog->windowFlags();
    dataDialog->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); // 对话框固定大小
    dataDialog->setUpdateRecord(curRec); // 更新对话框的数据和界面

    int ret = dataDialog->exec();
    if (ret == QDialog::Accepted)
    {
        QSqlRecord recData = dataDialog->getRecordData(); // 获取对话框返回的记录
        query.prepare("UPDATE employee SET Name=:Name, Gender=:Gender,"
                      " Birthday=:Birthday, Province=:Province,"
                      " Department=:Department, Salary=:Salary,"
                      " Memo=:Memo, Photo=:Photo "
                      " WHERE EmpNo = :ID");
        query.bindValue(":Name", recData.value("Name"));
        query.bindValue(":Gender", recData.value("Gender"));
        query.bindValue(":Birthday", recData.value("Birthday"));
        query.bindValue(":Province", recData.value("Province"));
        query.bindValue(":Department", recData.value("Department"));
        query.bindValue(":Salary", recData.value("Salary"));
        query.bindValue(":Memo", recData.value("Memo"));
        query.bindValue(":Photo", recData.value("Photo"));
        query.bindValue(":ID", empNo);
        if (!query.exec())
            QMessageBox::critical(this, "错误", "记录更新错误\n" + query.lastError().text());
        else
        {
            QString sqlStr = qryModel->query().executedQuery(); // 运行过的SELECT语句
            qryModel->setQuery(sqlStr); // 重新查询数据
        }
        // qryModel->query().exec(); // 数据模型重新查询数据，更新tableView显示内容
    }
    delete dataDialog;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tableView);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不可编辑
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 行选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // 单行选择
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actOpenDB_triggered()
{
    // “打开数据库”按钮
    QString aFile = QFileDialog::getOpenFileName(this, "选择文件", "", "SQLite数据库(*.db3)");
    if (aFile.isEmpty())
        return;
    DB = QSqlDatabase::addDatabase("QSQLITE"); // 添加SQLITE数据库驱动
    DB.setDatabaseName(aFile); // 设置数据库文件
    if (DB.open())
        selectData();
    else
        QMessageBox::warning(this, "错误", "打开数据库失败");
}


void MainWindow::on_actRecEdit_triggered()
{
    // 编辑当前记录
    int curRecNo = selModel->currentIndex().row();
    updateRecord(curRecNo);
}



void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    // 在tableView上双击某条记录，编辑当前记录
    int curRecNo = index.row();
    updateRecord(curRecNo);
}


void MainWindow::on_actRecInsert_triggered()
{
    QSqlQuery query;
    query.exec("SELECT * FROM employee WHERE EmpNo = -1"); // 查不出实际记录，只查询出字段信息
    QSqlRecord curRec = query.record(); // 获取当前记录，实际为空记录
    curRec.setValue("EmpNo", qryModel->rowCount() + 3000);

    TDialogData *dataDialog = new TDialogData(this); // 创建对话框
    Qt::WindowFlags flags = dataDialog->windowFlags();
    dataDialog->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint); // 对话框固定大小
    dataDialog->setInsertRecord(curRec); // 插入记录

    int ret = dataDialog->exec();
    if (ret == QDialog::Accepted)
    {
        QSqlRecord recData = dataDialog->getRecordData(); // 获取对话框返回的记录
        query.prepare("INSERT INTO employee (EmpNo,Name,Gender,Birthday,Province,"
                      " Department,Salary,Memo,Photo) "
                      " VALUES(:Name,:Gender,:Birthday,:Province,"
                      " :Department:Salary,:Memo,:Photo)");
        query.bindValue(":EmpNo", recData.value("EmpNo"));
        query.bindValue(":Name", recData.value("Name"));
        query.bindValue(":Gender", recData.value("Gender"));
        query.bindValue(":Birthday", recData.value("Birthday"));
        query.bindValue(":Province", recData.value("Province"));
        query.bindValue(":Department", recData.value("Department"));
        query.bindValue(":Salary", recData.value("Salary"));
        query.bindValue(":Memo", recData.value("Memo"));
        query.bindValue(":Photo", recData.value("Photo"));
        if (!query.exec())
            QMessageBox::critical(this, "错误", "插入记录错误\n" + query.lastError().text());
        else
        {
            QString sqlStr = qryModel->query().executedQuery(); // 运行过的SELECT语句
            qryModel->setQuery(sqlStr); // 重新查询数据
        }
    }
    delete dataDialog;
}


void MainWindow::on_actRecDelete_triggered()
{
    int curRecNo = selModel->currentIndex().row();
    QSqlRecord curRec = qryModel->record(curRecNo); // 获取当前记录
    if (curRec.isEmpty()) // 当前为空记录
        return;

    int empNo = curRec.value("EmpNo").toInt(); // 获取工号
    QSqlQuery query;
    query.prepare("DELETE FROM employee WHERE EmpNo = :ID");
    query.bindValue(":ID", empNo);
    if (!query.exec())
        QMessageBox::critical(this, "错误", "删除记录出现错误\n" + query.lastError().text());
    else
    {
        QString sqlStr = qryModel->query().executedQuery(); // 运行过的SELECT语句
        qryModel->setQuery(sqlStr); // 重新查询数据
    }
}


void MainWindow::on_actScan_triggered()
{
    /*// 遍历记录的方式
    QSqlQuery qryUpdate; // 用于临时执行SQL语句
    qryUpdate.prepare("UPDATE employee SET Salary=:Salary WHERE EmpNo = :ID");
    QSqlQuery qryEmpList;
    qryEmpList.setForwardOnly(true); // 设置为仅能前向移动,提高查询性能
    qryEmpList.exec("SELECT empNo,Salary FROM employee ORDER BY empNo");
    qryEmpList.first();
    while (qryEmpList.isValid()) // 当前记录有效
    {
        int empID = qryEmpList.value("empNo").toInt();
        float salary = 1000 + qryEmpList.value("Salary").toFloat();
        qryUpdate.bindValue(":ID", empID);
        qryUpdate.bindValue(":Salary", salary);
        qryUpdate.exec();
        qryEmpList.next(); // 移动到下一条记录，
    }

    qryModel->query().exec(); // 数据模型重新查询数据，更新tableView的显示
    QMessageBox::information(this, "提示", "涨工资计算完毕");*/

    // 直接执行SQL语句的方式
    QSqlQuery qryUpdate;
    qryUpdate.exec("UPDATE employee SET Salary=Salary+1000");
    QString sqlStr = qryModel->query().executedQuery(); // 运行过的SELECT语句
    qryModel->setQuery(sqlStr); // 重新查询数据
    //qryModel->query().exec(); // 数据模型重新查询数据，更新tableView的显示
    QMessageBox::information(this, "提示", "涨工资计算完毕");
}

