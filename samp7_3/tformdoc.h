#ifndef TFORMDOC_H
#define TFORMDOC_H

#include <QWidget>

namespace Ui {
class TFormDoc;
}

class TFormDoc : public QWidget
{
    Q_OBJECT

public:
    explicit TFormDoc(QWidget *parent = nullptr);
    ~TFormDoc();
    void loadFromFile(QString &aFileName); // 自定义函数
signals:
    void titleChanged(QString title); // 自定义信号

private slots:
    void on_actOpen_triggered();

    void on_actFont_triggered();

private:
    Ui::TFormDoc *ui;
};

#endif // TFORMDOC_H
