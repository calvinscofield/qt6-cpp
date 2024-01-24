#ifndef TMYDEVICE_H
#define TMYDEVICE_H

#include <QFile>
#include <QIODevice>
#include <QLineSeries>
#include <QObject>

class TMyDevice : public QIODevice
{
    Q_OBJECT
private:
    qint64 m_range = 4000; // 图表序列最多的数据点个数
    bool m_drawChart = true; // 是否需要绘制曲线
    QLineSeries *m_series; // 图表序列
    bool m_saveToFile = false; // 是否要保存到文件
    QString m_fileName; // 保存的文件名
    QFile m_file; // QFile对象
public:
    explicit TMyDevice(QObject *parent = nullptr);
    void openDAQ(qint64 pointsCount, bool drawChart, QLineSeries *series, bool saveToFile, QString filename); // 代替open()函数
    void closeDAQ(); // 代替close()函数


    // QIODevice interface
protected:
    qint64 readData(char *data, qint64 maxSize); // 从设备读取数据到缓冲区
    qint64 writeData(const char *data, qint64 maxSize); // 缓冲区数据写入设备
signals:
    void updateBlockSize(qint64 blockSize); // 在writeData()里发射的一个信号
};

#endif // TMYDEVICE_H
