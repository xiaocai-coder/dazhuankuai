    #ifndef PADDLE_H
    #define PADDLE_H

    #include <QRect>
    #include <QPainter>

    class Paddle
    {
    public:
        Paddle(int x, int y);
        void draw(QPainter *p);
        void moveLeft();
        void moveRight();
        QRect rect() const;
        void resetPosition(int x,int y);

    private:
        QRect paddleRect;
        int speed = 10;
    };

    #endif // PADDLE_H
