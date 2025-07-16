#include "brick.h"

Brick::Brick(int x, int y, int hits)
    : hitsRemaining(hits)
{
    brickRect = QRect(x, y, 50, 20);
}

QRect Brick::rect() const {//区域
    return brickRect;
}

bool Brick::isDestroyed() const {
    return hitsRemaining <= 0;
}

void Brick::hit() {//撞击
    hitsRemaining--;
}

int Brick::getHitsRemaining() const {
    return hitsRemaining;
}

void Brick::draw(QPainter *p) const
{
    if (isDestroyed()) return;

    QColor color;
    switch (hitsRemaining) {
    case 3: color = Qt::darkRed; break;
    case 2: color = Qt::blue; break;
    case 1: color = Qt::green; break;
    default: color = Qt::gray; break;
    }

    p->setBrush(color);
    p->drawRect(brickRect);
}
