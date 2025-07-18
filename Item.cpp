#include "Item.h"

Item::Item(int x, int y, ItemType t)
    : type(t), itemRect(x, y, 20, 20) {}

void Item::move() {
    itemRect.translate(0, dy);
}

void Item::draw(QPainter *p) {
    switch (type) {
    case ExtraLife: p->setBrush(Qt::red); break;
    case EnlargePaddle: p->setBrush(Qt::blue); break;
    case SpeedUpBall: p->setBrush(Qt::green); break;
    }
    p->drawEllipse(itemRect);
}

QRect Item::rect() const { return itemRect; }

ItemType Item::getType() const { return type; }

bool Item::isCaughtBy(Paddle *p) {
    return itemRect.intersects(p->rect());
}
