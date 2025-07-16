#ifndef BALL_H
#define BALL_H

#include <QRect>
#include <QPainter>
#include "paddle.h"
#include "brick.h"

class Ball
{
public:
    Ball(int x, int y);
    void move();
    void draw(QPainter *p);
    void bounceX();
    void bounceY();
    bool collidesWith(Paddle *p);
    bool collidesWith(Brick *b);
    QRect rect() const;      // 获取球的矩形区域
    int centerX() const;     // （可选）用于反弹角度判断
    int y() const;           // （可选）返回球的 y 坐标
    void resetPosition(int ,int y);
private:
    QRect ballRect;
    int dx = 4, dy = -4;
};

#endif // BALL_H
