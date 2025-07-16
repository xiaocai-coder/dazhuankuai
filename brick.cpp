#include "brick.h"

Brick::Brick(int x, int y)
{
    brickRect = QRect(x, y, 50, 20);
}

void Brick::draw(QPainter *p)
{
    p->setBrush(Qt::green);
    p->drawRect(brickRect);
}

QRect Brick::rect() const
{
    return brickRect;
}

bool Brick::isDestroyed() const
{
    return destroyed;
}

void Brick::destroy()
{
    destroyed = true;
}
