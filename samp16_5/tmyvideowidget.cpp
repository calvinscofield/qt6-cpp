#include "tmyvideowidget.h"

#include <QKeyEvent>

TMyVideoWidget::TMyVideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{}

void TMyVideoWidget::setMediaPlayer(QMediaPlayer *player)
{
    // 设置播放器
    m_player = player;
}


void TMyVideoWidget::mousePressEvent(QMouseEvent *event)
{
    // 鼠标事件处理函数，点击时暂停播放或继续播放
    if (event->button() == Qt::LeftButton)
    {
        if (m_player->playbackState() == QMediaPlayer::PlayingState)
            m_player->pause();
        else
            m_player->play();
    }
    QVideoWidget::mousePressEvent(event);
}

void TMyVideoWidget::keyPressEvent(QKeyEvent *event)
{
    // 按键事件处理函数，按ESC键退出全屏状态
    if ((event->key() == Qt::Key_Escape) && (isFullScreen()))
    {
        setFullScreen(false); // 退出全屏状态
        event->accept();
        QVideoWidget::keyPressEvent(event);
    }
}
