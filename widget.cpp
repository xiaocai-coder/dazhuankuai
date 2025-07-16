#include "widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(600, 400);//窗口大小
    paddle = new Paddle(this->width() / 2 - 40, this->height() - 30);//创建挡板和球
    ball = new Ball(this->width() / 2, this->height() / 2);

    resetBricks();


    scoreFont.setFamily("Arial");//字体，得分
    scoreFont.setPointSize(14);
    scoreFont.setBold(true);


    timer = new QTimer(this);//启动定时器
    connect(timer, &QTimer::timeout, this, &GameWidget::updateGame);
    timer->start(16);

//    restartButton = new QPushButton("重新开始", this);
//    restartButton->setGeometry(width() - 110, height() - 40, 100, 30);
//    connect(restartButton, &QPushButton::clicked, this, [=]() {
//        score = 0;
//        lives = 3;
//        resetBricks();


//        paddle->resetPosition(width() / 2 - 40, height() - 30);
//        ball->resetPosition(width() / 2, height() / 2);

//        //  状态清空
//        leftPressed = false;
//        rightPressed = false;

//        timer->start();
//        update();
//    });
}

void GameWidget::resetBricks()
{
    bricks.clear();
    int rows = 5, cols = 10;
    int brickWidth = 50, brickHeight = 20;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            bricks.append(new Brick(j * (brickWidth + 5) + 20, i * (brickHeight + 5) + 20));
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paddle->draw(&p);
    ball->draw(&p);
    for (auto brick : bricks)
        if (!brick->isDestroyed())
            brick->draw(&p);

    // 绘制得分
        p.setFont(scoreFont);
        p.setPen(Qt::black);
        p.drawText(10, 20, QString("Score: %1").arg(score));
        p.drawText(10, 40, QString("Lives: %1").arg(lives));
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        leftPressed = true;
    else if (event->key() == Qt::Key_Right)
        rightPressed = true;
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        leftPressed = false;
    else if (event->key() == Qt::Key_Right)
        rightPressed = false;
}

void GameWidget::updateGame()
{
    // 1. 移动挡板（根据按键状态）
    if (leftPressed) paddle->moveLeft();
    if (rightPressed) paddle->moveRight();

    // 2. 移动小球
    ball->move();

    // 3. 检测小球是否与挡板碰撞
    if (ball->collidesWith(paddle)) {
        ball->bounceY();
    }

    // 4. 检测小球是否与砖块碰撞
    for (auto brick : bricks) {
        if (!brick->isDestroyed() && ball->collidesWith(brick)) {
            brick->destroy();      // 销毁砖块
            ball->bounceY();       // 反弹球
            score += 10;           // 加分
            break;                 // 防止多个砖块同时被击中
        }
    }

    // 5. 检测球是否掉到底部（后续可加 Game Over 逻辑）
    if (ball->rect().bottom() >= height()) {
        lives--;
        if (lives <= 0) {
            timer->stop();
            QMessageBox::information(this, "Game Over", "你输了！");
            return;
        } else {
            resetBallAndPaddle();
        }
    }
    // 6. 检查是否通关（所有砖块被消灭）
    bool win = true;
    for (auto brick : bricks) {
        if (!brick->isDestroyed()) {
            win = false;
            break;
        }
    }

    if (win) {
        timer->stop();
        QMessageBox::information(this, "Congratulations", "你赢了！");
        return;
    }

    // 7. 重新绘制界面
    update();
}

void GameWidget::resetBallAndPaddle()
{
    paddle->resetPosition(width() / 2 - 40, height() - 30);
    ball->resetPosition(width() / 2, height() / 2);
}
