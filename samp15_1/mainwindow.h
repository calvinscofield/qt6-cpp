#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnGetHostInfo_clicked();

    void on_btnLookup_clicked();

    void do_lookedUpHostInfo(const QHostInfo &host);

    void on_btnAllInterface_clicked();

    void on_btnAllAddress_clicked();

private:
    Ui::MainWindow *ui;
    QString protocolName(QAbstractSocket::NetworkLayerProtocol protocol);
    QString interfaceType(QNetworkInterface::InterfaceType type);
};
#endif // MAINWINDOW_H
