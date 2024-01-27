#include "tmylabel.h"

#include <QMouseEvent>

TMyLabel::TMyLabel(QWidget *parent) : QLabel(parent) {}


void TMyLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked();
    event->accept();
}
