#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>


void MainWindow::iniModelData(QStringList &aFileContent)
{
    int rowCnt = aFileContent.size(); // 文本行数，第一行是标题
    m_model->setRowCount(rowCnt - 1); // 实际数据行数
    QString header = aFileContent.at(0); // 第一行是表头文字
    // 以一个或多个空格、制表符等分隔符隔开的字符串，分割为一个StringList
    QStringList headerList = header.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    m_model->setHorizontalHeaderLabels(headerList); // 设置表头文字

    // 设置表格数据
    int j;
    QStandardItem *aItem;
    for (int i = 1; i < rowCnt; i++)
    {
        QString aLineText = aFileContent.at(i); // 获取数据区的一行
        // 以一个或多个空格、制表符等分隔符隔开的字符串，分割为一个StringList
        QStringList tmpList = aLineText.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        for (j = 0; j < FixedColumnCount - 1; j++)
        {
            // 不包含最后一列
            aItem = new QStandardItem(tmpList.at(j));
            m_model->setItem(i - 1, j, aItem);
        }

        aItem = new QStandardItem(headerList.at(j)); // 最后一列
        aItem->setCheckable(true); // 设置为Checkable
        aItem->setBackground(QBrush(Qt::yellow));
        if (tmpList.at(j) == "0")
            aItem->setCheckState(Qt::Unchecked);
        else
            aItem->setCheckState(Qt::Checked);
        m_model->setItem(i - 1, j, aItem);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new QStandardItemModel(2, FixedColumnCount, this); // 创建数据模型
    m_selection = new QItemSelectionModel(m_model, this); // 创建选择模型，并设置数据模型
    // 选择当前单元格变化时的信号与槽
    connect(m_selection, &QItemSelectionModel::currentChanged, this, &MainWindow::do_currentChanged);
    // 为tableView设置数据模型和选择模型
    ui->tableView->setModel(m_model); // 设置数据模型
    ui->tableView->setSelectionModel(m_selection); // 设置选择模型
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    intSpinDelegate = new TSpinBoxDelegate(this);
    ui->tableView->setItemDelegateForColumn(0, intSpinDelegate); // 测深

    floatSpinDelegate = new TFloatSpinDelegate(this);
    ui->tableView->setItemDelegateForColumn(1, floatSpinDelegate); // 垂深
    ui->tableView->setItemDelegateForColumn(2, floatSpinDelegate); // 方位
    ui->tableView->setItemDelegateForColumn(3, floatSpinDelegate); // 总位移

    comboDelegate = new TComboBoxDelegate(this);
    QStringList strList;
    strList << "优" << "良" << "一般" << "不合格";
    comboDelegate->setItems(strList, false);
    ui->tableView->setItemDelegateForColumn(4, comboDelegate); // 固井质量

    setCentralWidget(ui->splitter);
    // 创建状态栏组件
    labCurFile = new QLabel("当前文件：", this);
    labCurFile->setMinimumWidth(200);
    labCellPos = new QLabel("当前单元格：", this);
    labCellPos->setMinimumWidth(180);
    labCellPos->setAlignment(Qt::AlignHCenter);
    labCellText = new QLabel("单元格内容：", this);
    labCellText->setMinimumWidth(150);
    ui->statusBar->addWidget(labCurFile);
    ui->statusBar->addWidget(labCellPos);
    ui->statusBar->addWidget(labCellText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (current.isValid())
    {
        labCellPos->setText(QString::asprintf("当前单元格：%d行，%d列", current.row(), current.column()));
        QStandardItem *aItem = m_model->itemFromIndex(current);
        // 通过模型索引获得项的QStandardItem对象指针
        labCellText->setText("单元格内容：" + aItem->text());
        QFont font = aItem->font();
        ui->actFontBold->setChecked(font.bold());
    }
}

void MainWindow::on_actOpen_triggered()
{
    QString curPath = QCoreApplication::applicationDirPath(); // 获取应用程序的路径
    QString aFileName = QFileDialog::getOpenFileName(this, "打开一个文件", curPath, "数据文件(*.txt);;所有文件(*.*)");
    if (aFileName.isEmpty())
        return;

    QStringList aFileContent;
    QFile aFile(aFileName);
    if (aFile.open(QIODevice::ReadOnly | QIODevice::Text)) // 以只读文本方式打开文件
    {
        QTextStream aStream(&aFile); // 用文本流读取文件
        ui->plainTextEdit->clear();
        while (!aStream.atEnd())
        {
            QString str = aStream.readLine(); // 读取文件的一行
            ui->plainTextEdit->appendPlainText(str);
            aFileContent.append(str);
        }
        aFile.close();
        labCurFile->setText("当前文件：" + aFileName); // 状态栏显示
        ui->actAppend->setEnabled(true);
        ui->actInsert->setEnabled(true);
        ui->actDelete->setEnabled(true);
        ui->actSave->setEnabled(true);
        iniModelData(aFileContent); // 用字符串列表内容初始化数据模型的数据
    }
}


void MainWindow::on_actAppend_triggered()
{
    // “添加行”按钮
    QList<QStandardItem *> aItemList;
    QStandardItem *aItem;
    for (int i = 0; i < FixedColumnCount - 1; i++) // 不包含最后一列
    {
        aItem = new QStandardItem("0");
        aItemList <<  aItem; // 添加到列表
    }
    // h获取最后一列的表头文字
    QString str = m_model->headerData(m_model->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    aItem = new QStandardItem(str);
    aItem->setCheckable(true);
    aItemList << aItem; // 添加到列表

    m_model->insertRow(m_model->rowCount(), aItemList); // 插入一行
    QModelIndex curIndex = m_model->index(m_model->rowCount() - 1, 0); // 获取模型索引
    m_selection->clearSelection();
    m_selection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
}


void MainWindow::on_actDelete_triggered()
{
    // “删除行”按钮
    QModelIndex curIndex = m_selection->currentIndex(); // 获取当前单元格的模型索引
    if (curIndex.row() == m_model->rowCount() - 1) // 最后一行
        m_model->removeRow(curIndex.row()); // 删除最后一行
    else
    {
        m_model->removeRow(curIndex.row()); // 删除一行，并重新设置当前选择项
        m_selection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
    }
}


void MainWindow::on_actAlignLeft_triggered()
{
    // 设置文字居左对齐
    if (!m_selection->hasSelection()) // 没有选择的项
        return;
    // 获取选择的单元格的模型索引列表，可以多选
    QModelIndexList IndexList = m_selection->selectedIndexes();
    for (int i = 0; i < IndexList.count(); i++)
    {
        QModelIndex aIndex = IndexList.at(i); // 获取一个模型索引
        QStandardItem *aItem = m_model->itemFromIndex(aIndex); // 获取一个单元格的项
        aItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }
}


void MainWindow::on_actFontBold_triggered(bool checked)
{
    // 设置字体为粗体
    if (!m_selection->hasSelection())
        return;
    // 获取选择单元格的模型索引列表
    QModelIndexList selectedIndex = m_selection->selectedIndexes();
    for (int i = 0; i < selectedIndex.count(); i++)
    {
        QModelIndex aIndex = selectedIndex.at(i); // 获取一个模型索引
        QStandardItem *aItem = m_model->itemFromIndex(aIndex); // 获取项
        QFont font = aItem->font();
        font.setBold(checked);
        aItem->setFont(font);
    }
}


void MainWindow::on_actModelData_triggered()
{
    // 模型数据导出到plainTextEdit中显示
    ui->plainTextEdit->clear();
    QStandardItem *aItem;
    QString str;
    // 获取表头文字
    for (int i = 0; i < m_model->columnCount(); i++)
    {
        aItem = m_model->horizontalHeaderItem(i); // 获取表头的一个项
        str = str + aItem->text() + "\t"; // 用制表符分隔文字
    }
    ui->plainTextEdit->appendPlainText(str);
    // 获取数据区的每行
    for (int i = 0; i < m_model->rowCount(); i++)
    {
        str = "";
        for (int j = 0; j < m_model->columnCount() - 1; j++)
        {
            aItem = m_model->item(i, j);
            str = str + aItem->text() + QString::asprintf("\t"); // 以制表符分隔
        }
        aItem = m_model->item(i, FixedColumnCount - 1); // 最后一行是逻辑型数据
        if (aItem->checkState() == Qt::Checked)
            str = str + "1";
        else
            str = str + "0";
        ui->plainTextEdit->appendPlainText(str);
    }
}


void MainWindow::on_actInsert_triggered()
{
    // 插入行
    QList<QStandardItem *> aItemList;  // QStandardItem的列表类
    QStandardItem *aItem;
    for (int i = 0;i < FixedColumnCount - 1; i++)
    {
        aItem = new QStandardItem("0"); // 新建一个QStandardItem
        aItemList << aItem; // 添加到列表类
    }
    // aItem = new QStandardItem("优"); // 新建一个QStandardItem
    // aItemList << aItem; // 添加到列表类

    QString str; // 获取表头文字
    str = m_model->headerData(m_model->columnCount() - 1, Qt::Horizontal, Qt::DisplayRole).toString();
    aItem = new QStandardItem(str); // 创建Item
    aItem->setCheckable(true); // 设置为可使用CheckBox
    aItemList << aItem; // 添加到列表类


    QModelIndex curIndex = m_selection->currentIndex(); // 获取当前选中项的模型索引
    m_model->insertRow(curIndex.row(), aItemList);  // 在当前行的前面插入一行
    m_selection->clearSelection(); // 清除已有选择
    m_selection->setCurrentIndex(curIndex, QItemSelectionModel::Select);
}


void MainWindow::on_actSave_triggered()
{
    // 保存为文件
    QString curPath = QCoreApplication::applicationDirPath(); // 获取应用程序的路径
    // 调用打开文件对话框选择一个文件
    QString aFileName = QFileDialog::getSaveFileName(this, tr("选择一个文件"), curPath, "井斜数据文件(*.txt);;所有文件(*.*)");

    if (aFileName.isEmpty()) // 未选择文件，退出
        return;

    QFile aFile(aFileName);
    if (!(aFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)))
        return; // 以读写、覆盖原有内容方式打开文件

    QTextStream aStream(&aFile); // 用文本流读取文件

    QStandardItem *aItem;
    int i, j;
    QString str;

    ui->plainTextEdit->clear();

    // 获取表头文字
    for (i = 0; i < m_model->columnCount(); i++)
    {
        aItem = m_model->horizontalHeaderItem(i); // 获取表头的项数据
        str = str + aItem->text() + "\t\t"; // 以TAB见隔开
    }
    aStream << str << "\n"; // 文件里需要加入换行符\n
    ui->plainTextEdit->appendPlainText(str);

    // 获取数据区文字
    for (i = 0; i < m_model->rowCount(); i++)
    {
        str = "";
        for (j = 0; j < m_model->columnCount() - 1; j++)
        {
            aItem = m_model->item(i, j);
            str = str + aItem->text() + QString::asprintf("\t\t");
        }

        aItem = m_model->item(i, j); // 最后一列是逻辑型
        if (aItem->checkState() == Qt::Checked)
            str = str + "1";
        else
            str = str + "0";

        ui->plainTextEdit->appendPlainText(str);
        aStream << str << "\n";
    }
}


void MainWindow::on_actAlignCenter_triggered()
{
    // 文字居中对齐
    if (!m_selection->hasSelection())
        return;

    QModelIndexList selectedIndex = m_selection->selectedIndexes();

    QModelIndex aIndex;
    QStandardItem *aItem;

    for (int i = 0; i < selectedIndex.count(); i++)
    {
        aIndex = selectedIndex.at(i);
        aItem = m_model->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}


void MainWindow::on_actAlignRight_triggered()
{
    // 设置文字居右对齐
    if (!m_selection->hasSelection())
        return;

    QModelIndexList selectedIndex = m_selection->selectedIndexes();

    QModelIndex aIndex;
    QStandardItem *aItem;

    for (int i = 0; i < selectedIndex.count(); i++)
    {
        aIndex = selectedIndex.at(i);
        aItem = m_model->itemFromIndex(aIndex);
        aItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
}

