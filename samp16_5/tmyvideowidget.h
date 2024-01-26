#ifndef TMYVIDEOWIDGET_H
#define TMYVIDEOWIDGET_H

#include <QMediaPlayer>
#include <QVideoWidget>

class TMyVideoWidget : public QVideoWidget
{
    Q_OBJECT
private:
    QMediaPlayer *m_player;
public:
    TMyVideoWidget(QWidget *parent = nullptr);
    void setMediaPlayer(QMediaPlayer *player);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // TMYVIDEOWIDGET_H
