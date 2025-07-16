#ifndef BRICK_H
#define BRICK_H

#include <QRect>
#include <QPainter>

class Brick
{
public:
    Brick(int x, int y, int hits = 1); // 新增默认参数：耐久度

    QRect rect() const;
    void draw(QPainter *p) const;
    bool isDestroyed() const;
    void hit(); // 受到一次撞击
    int getHitsRemaining() const;

private:
    QRect brickRect;
    int hitsRemaining; // 耐久度（打中几次才销毁）
};

#endif // BRICK_H
