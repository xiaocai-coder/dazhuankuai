#ifndef ITEM_H
#define ITEM_H

#include <QRect>
#include <QPainter>
#include "paddle.h"

enum ItemType { ExtraLife, EnlargePaddle, SpeedUpBall };

class Item {
public:
    Item(int x, int y, ItemType type);
    void move(); // 向下移动
    void draw(QPainter *p);
    QRect rect() const;
    ItemType getType() const;
    bool isCaughtBy(Paddle *p);

private:
    QRect itemRect;
    ItemType type;
    int dy = 3;
};

#endif // ITEM_H
