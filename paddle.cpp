#include "paddle.h"

Paddle::Paddle(int x, int y)
{
    paddleRect = QRect(x, y, 80, 10);
}

void Paddle::draw(QPainter *p)
{
    p->setBrush(Qt::blue);
    p->drawRect(paddleRect);
}

void Paddle::moveLeft()
{
    if (paddleRect.left() > 0)
        paddleRect.translate(-speed, 0);
}

void Paddle::moveRight()
{
    if (paddleRect.right() < 600)
        paddleRect.translate(speed, 0);
}

QRect Paddle::rect() const
{
    return paddleRect;
}

void Paddle::resetPosition(int x, int y)
{
    paddleRect.moveTo(x, y);
}


void Paddle::enlarge() {
    paddleRect.setWidth(paddleRect.width() + 30);
}

