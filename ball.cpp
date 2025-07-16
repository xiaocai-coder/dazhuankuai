#include "ball.h"

Ball::Ball(int x, int y)
{
    ballRect = QRect(x, y, 10, 10);
}

void Ball::draw(QPainter *p)
{
    p->setBrush(Qt::red);
    p->drawEllipse(ballRect);
}

void Ball::move()
{
    ballRect.translate(dx, dy);
    if (ballRect.left() <= 0 || ballRect.right() >= 600) bounceX();
    if (ballRect.top() <= 0) bounceY();
}

void Ball::bounceX() { dx = -dx; }
void Ball::bounceY() { dy = -dy; }

bool Ball::collidesWith(Paddle *p)
{
    return ballRect.intersects(p->rect());
}

bool Ball::collidesWith(Brick *b)
{
    return ballRect.intersects(b->rect());
}
QRect Ball::rect() const
{
    return ballRect;
}

int Ball::centerX() const
{
    return ballRect.center().x();
}

int Ball::y() const
{
    return ballRect.y();
}

void Ball::resetPosition(int x, int y)
{
    ballRect.moveTo(x, y);
    dx = 4; dy = -4; // 方向重置（可调）
}
