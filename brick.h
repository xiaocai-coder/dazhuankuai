#ifndef BRICK_H
#define BRICK_H

#include <QRect>
#include <QPainter>

class Brick
{
public:
    Brick(int x, int y);
    void draw(QPainter *p);
    QRect rect() const;
    bool isDestroyed() const;
    void destroy();

private:
    QRect brickRect;
    bool destroyed = false;
};

#endif // BRICK_H
