#ifndef TFLOATSPINDELEGATE_H
#define TFLOATSPINDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TFloatSpinDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TFloatSpinDelegate(QObject *parent = nullptr);

    // 自定义代理必须重新实现以下4个函数
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TFLOATSPINDELEGATE_H
