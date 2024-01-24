#include "tmydevice.h"

#include <QFile>

TMyDevice::TMyDevice(QObject *parent)
    : QIODevice{parent}
{}

void TMyDevice::openDAQ(qint64 pointsCount, bool drawChart, QLineSeries *series, bool saveToFile, QString filename)
{
    m_range= pointsCount; // 序列最多的数据点个数
    m_drawChart = drawChart; // 是否绘制曲线
    m_series = series; // 序列
    m_saveToFile = saveToFile; // 是否保存到文件
    m_fileName = filename; // 保存的文件名
    this->open(QIODeviceBase::WriteOnly); // 打开本设备
    // 如果需要保存到文件，就需要打开文件设备，以便写入数据
    if (!m_saveToFile)
        return;
    if (QFile::exists(m_fileName))
        QFile::remove(m_fileName); // 删除原有文件
    m_file.setFileName(m_fileName);
    m_file.open(QIODeviceBase::WriteOnly); // 打开文件设备
}

void TMyDevice::closeDAQ()
{
    if (m_saveToFile)
        m_file.close(); // 关闭文件设备
    this->close(); // 关闭本I/O设备
}


qint64 TMyDevice::readData(char *data, qint64 maxSize)
{
    // 读数据，不做处理
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 TMyDevice::writeData(const char *data, qint64 maxSize)
{
    // 写缓冲区的数据
    if (m_saveToFile) // 需要保存到文件
        m_file.write(data, maxSize); // 将缓冲区的数据写入文件设备

    if (m_drawChart) // 需要绘制曲线
    {
        QList<QPointF> points;
        points.reserve(m_range); // 预分配空间，可加快处理速度
        // calvin注释：书本上本行末尾的','有误，应为';'。
        int oldCount = m_series->points().size(); // 现在序列的数据点数
        if (oldCount < m_range) // 序列的数据未满
            points = m_series->points();
        else
        {
            for (int i = maxSize; i < oldCount; i++)
                points.append(QPointF(i - maxSize, m_series->points().at(i).y()));
        }
        int curCount = points.size();
        for (int k = 0; k < maxSize; k++) // 数据块的数据填充序列的尾部
            points.append(QPointF(k + curCount, (quint8)(data[k])));
        m_series->replace(points);
    }
    emit updateBlockSize(maxSize); // 发射信号
    return maxSize;
}
