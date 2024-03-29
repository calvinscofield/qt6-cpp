#ifndef WIDGET_H
#define WIDGET_H

#include <QSoundEffect>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    QSoundEffect *player1;
    QSoundEffect *player2;
    QString appPath;
    QPixmap pixBackground;
    void defense(QString weapon);
    void attack(QString weapon);

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_12_clicked();
};
#endif // WIDGET_H
