#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QRandomGenerator>

void MainWindow::createItemsARow(int rowNo, QString name, QString sex, QDate birth, QString nation, bool isPM, int score)
{
    uint studID = 202105000; // 学号基数
    // 姓名
    QTableWidgetItem *item = new QTableWidgetItem(name, MainWindow::ctName); // type为ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    studID += rowNo; // 学号 = 基数 + 行号
    item->setData(Qt::UserRole, QVariant(studID)); // 设置studID为用户数据
    ui->tableInfo->setItem(rowNo, MainWindow::colName, item); // 为单元格设置item
    // 性别
    QIcon icon;
    if (sex == "男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");
    item = new QTableWidgetItem(sex, MainWindow::ctSex); // type 为ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled; // “性别”单元格不允许编辑
    item->setFlags(flags);
    ui->tableInfo->setItem(rowNo, MainWindow::colSex, item);
    // 出生日期
    QString str = birth.toString("yyyy-MM-dd"); // 日期转换为字符串
    item = new QTableWidgetItem(str, MainWindow::ctBirth); // type为ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, MainWindow::colBirth, item);
    // 民族
    item = new QTableWidgetItem(nation, MainWindow::ctNation); // type为ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, MainWindow::colNation, item);
    // 是否党员
    item = new QTableWidgetItem("党员", MainWindow::ctPartyM); // type为ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    // “党员”单元格不允许编辑，但可以更改复选状态
    flags = Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    item->setFlags(flags);
    if (isPM)
        item->setCheckState(Qt::Checked); // 设置复选框
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow)); // 设置背景色
    ui->tableInfo->setItem(rowNo, MainWindow::colPartyM, item);
    // 分数
    str.setNum(score);
    item = new QTableWidgetItem(str, MainWindow::ctScore); // type为ctScore
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, MainWindow::colScore, item);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 状态栏初始化
    labCellIndex = new QLabel("当前单元格坐标：", this);
    labCellIndex->setMinimumWidth(250);
    labCellType = new QLabel("当前单元格类型：", this);
    labCellType->setMinimumWidth(200);
    labStudID = new QLabel("学生ID：", this);
    labStudID->setMinimumWidth(200);
    ui->statusBar->addWidget(labCellIndex); // 添加到状态栏
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudID);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSetHeader_clicked()
{
    QStringList headerText;
    headerText << "姓名" << "性别" << "出生日期" << "民族" << "分数" << "是否党员";
    // ui->tableInfo->setHorizontalHeaderLabels(headerText); // 只设置标题
    ui->tableInfo->setColumnCount(headerText.size()); // 设置表格列表
    for (int i = 0; i < ui->tableInfo->columnCount(); i++)
    {
        QTableWidgetItem *headerItem = new QTableWidgetItem(headerText.at(i)); // 创建item
        QFont font = headerItem->font(); // 获取原有字体设置
        font.setBold(true); // 设置为粗体
        font.setPointSize(11); // 字体大小
        headerItem->setForeground(QBrush(Qt::red)); // 设置文字颜色
        headerItem->setFont(font); // 设置字体
        ui->tableInfo->setHorizontalHeaderItem(i, headerItem); // 设置表头单元格的item
    }
}


void MainWindow::on_btnIniData_clicked()
{
    QDate birth(2001, 4, 6); // 初始化日期
    ui->tableInfo->clearContents(); // 只清除工作区，不清除表头
    for (int i = 0; i < ui->tableInfo->rowCount(); i++)
    {
        QString strName = QString("学生%1").arg(i);
        QString strSex = ((i % 2) == 0) ? "男" : "女";
        bool isParty = ((i % 2) == 0) ? false : true;
        int score = QRandomGenerator::global()->bounded(60, 100); // 随机数范围为[60,100)
        createItemsARow(i, strName, strSex, birth, "汉族", isParty, score); // 为某一行创建items
        birth = birth.addDays(20); // 日期加20天
    }
}


void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);
    QTableWidgetItem *item = ui->tableInfo->item(currentRow, currentColumn); // 获取item
    if (item == nullptr)
        return;
    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行，%d 列", currentRow, currentColumn));
    int cellType = item->type(); // 获取单元格的类型
    labCellType->setText(QString::asprintf("当前单元格类型：%d", cellType));

    item = ui->tableInfo->item(currentRow, MainWindow::colName); // 当前行第一列的item
    uint ID = item->data(Qt::UserRole).toUInt(); // 读取用户数据
    labStudID->setText(QString::asprintf("学生ID：%d", ID)); // 学生ID
}


void MainWindow::on_btnInsertRow_clicked()
{
    // “插入行”按钮
    int curRow = ui->tableInfo->currentRow(); // 当前行的行号
    ui->tableInfo->insertRow(curRow); // 插入一行，但不会自动为单元格创建item
    createItemsARow(curRow, "新学生", "男", QDate::fromString("2002-10-1", "yyyy-M-d"), "苗族", true, 80);
}


void MainWindow::on_btnAppendRow_clicked()
{
    // “添加行”按钮
    int curRow = ui->tableInfo->rowCount(); // 当前行的行号
    ui->tableInfo->insertRow(curRow); // 在表格末尾添加一行
    createItemsARow(curRow, "新生", "女", QDate::fromString("2002-6-5", "yyyy-M-d"), "满族", false, 76);
}


void MainWindow::on_btnDelCurRow_clicked()
{
    // “删除当前行”按钮
    int curRow = ui->tableInfo->currentRow(); // 当前行的行号
    ui->tableInfo->removeRow(curRow); // 删除当前行及其items
}


void MainWindow::on_chkBoxTabEditable_clicked(bool checked)
{
    if (checked) // 允许编辑，双击或获取焦点后点击可进入编辑状态
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    else
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers); // 不允许编辑
}


void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{
    // 是否显示水平表头
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}


void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{
    // 是否显示垂直表头
    ui->tableInfo->verticalHeader()->setVisible(checked);
}


void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{
    // 行的底色交替采用不同颜色
    ui->tableInfo->setAlternatingRowColors(checked);
}


void MainWindow::on_rBtnSelectItem_clicked()
{
    // 选择方式：单元格选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
}


void MainWindow::on_rBtnSelectRow_clicked()
{
    // 选择方式：行选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}


void MainWindow::on_btnReadToEdit_clicked()
{
    QTableWidgetItem *item;
    ui->textEdit->clear();
    for (int i = 0; i < ui->tableInfo->rowCount(); i++) // 逐行处理
    {
        QString str = QString::asprintf("第 %d 行：", i + 1);
        for (int j = 0; j < ui->tableInfo->columnCount() - 1; j++) // 逐列处理，最后一列单独处理
        {
            item = ui->tableInfo->item(i, j); // 获取单元格的item
            str = str + item->text() + "    "; //字符串拼接
        }
        item = ui->tableInfo->item(i, colPartyM); // 最后一列
        if (item->checkState() == Qt::Checked) // 根据check状态显示文字
            str = str + "党员";
        else
            str = str + "群众";
        ui->textEdit->appendPlainText(str); // 添加到编辑框作为一行
    }
}


void MainWindow::on_btnSetRows_clicked()
{
    // 设置行数，设置的行数为数据区的行数，不含表头
    ui->tableInfo->setRowCount(ui->spinRowCount->value()); // 设置数据区行数
    ui->tableInfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked()); // 设置交替行背景颜色
}


void MainWindow::on_btnAutoHeght_clicked()
{
    ui->tableInfo->resizeRowsToContents();
}


void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableInfo->resizeColumnsToContents();
}

