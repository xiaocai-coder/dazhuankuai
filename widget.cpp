#include "widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include "Item.h"
#include <QTime>

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


//    timer = new QTimer(this);//启动定时器
//    connect(timer, &QTimer::timeout, this, &GameWidget::updateGame);
//    timer->start(16);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWidget::updateGame);


    restartButton = new QPushButton("重新开始", this);
    restartButton->setGeometry(width() - 110, height() - 40, 100, 30);
    connect(restartButton, &QPushButton::clicked, this, [=]() {
        score = 0;
        lives = 3;
        resetBricks();


        paddle->resetPosition(width() / 2 - 40, height() - 30);
        ball->resetPosition(width() / 2, height() / 2);

        //  状态清空
        leftPressed = false;
        rightPressed = false;

        timer->start(16);
        update();
    });

    setFocusPolicy(Qt::StrongFocus);

    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile(":/prefix1/inmage/yinxiao.mp3"));
    player->setVolume(50);



}

void GameWidget::resetBricks()
{
    bricks.clear();
    int rows = 5, cols = 10;
    int brickWidth = 50, brickHeight = 20;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int hits = 1;

            // 根据当前关卡设置砖块耐久度分布
            if (currentLevel == 1) {
                // 第1关：上强化砖
                if (i < 1) hits = 3;
                else if (i < 3) hits = 2;
            }
            else if (currentLevel == 2) {
                // 第2关：交错强化砖
                hits = ((i + j) % 3) + 1; // 1~3之间变化
            }
            else if (currentLevel == 3) {
                // 第3关：下方更硬
                if (i >= 4) hits = 3;
                else if (i >= 2) hits = 2;
                else hits = 1;
            }
            else {
                // 默认（或未定义关卡）使用第1关的布局
                if (i < 1) hits = 3;
                else if (i < 3) hits = 2;
            }

            bricks.append(new Brick(j * (brickWidth + 5) + 20,
                                     i * (brickHeight + 5) + 20,
                                     hits));
        }
    }
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paddle->draw(&p);
    ball->draw(&p);
    for (auto brick : bricks)
        if (!brick->isDestroyed())
            brick->draw(&p);

    // 3. 绘制游戏状态信息（关卡，得分，生命）
    p.setPen(Qt::black);
    p.setFont(scoreFont);  // 你之前定义的字体对象

    // 左上角显示得分、生命和关卡
    QString info = QString("关卡: %1    得分: %2    生命: %3")
                        .arg(currentLevel)
                        .arg(score)
                        .arg(lives);
    p.drawText(10, 20, info);
    for (Item* item : items)
        item->draw(&p);

        //绘制暂停
        if (isPaused) {
                QFont pauseFont("Arial", 20, QFont::Bold);
                p.setFont(pauseFont);
                p.setPen(Qt::red);
                p.drawText(rect(), Qt::AlignCenter, "游戏暂停\n按空格继续");
            }

}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        leftPressed = true;
    else if (event->key() == Qt::Key_Right)
        rightPressed = true;

    else if (event->key() == Qt::Key_Space)
        {
            isPaused = !isPaused;
            if (isPaused)
                timer->stop(); // 暂停
            else
                timer->start(16); // 恢复
            update(); // 立即刷新屏幕，显示或清除“暂停”提示
        }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        leftPressed = false;
    else if (event->key() == Qt::Key_Right)
        rightPressed = false;
}

void GameWidget::startGame()
{
    score = 0;
    lives = 3;
    resetBricks();
    paddle->resetPosition(width() / 2 - 40, height() - 30);
    ball->resetPosition(width() / 2, height() / 2);
    leftPressed = false;
    rightPressed = false;
    qsrand(QTime::currentTime().msec());


    timer->start(16);
    update();
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
            brick->hit();          // 先减命
            ball->bounceY();
            if (brick->isDestroyed()) {
                score += 10;
                player->play();

                // 随机生成道具（30% 概率）
                if (qrand() % 100 < 30) {
                    ItemType type = static_cast<ItemType>(qrand() % 3);
                    items.append(new Item(brick->rect().center().x(), brick->rect().center().y(), type));
                }
            }
                // 只在砖块消失时加分
            player->play();;
            break;
        }
    }

    //5.判断球是否与道具碰撞




    // 6. 检测球是否掉到底部（后续可加 Game Over 逻辑）
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

    // 7. 检查通关条件（所有砖块被打掉）
        bool allDestroyed = true;
        for (auto brick : bricks) {
            if (!brick->isDestroyed()) {
                allDestroyed = false;
                break;
            }
        }

        if (allDestroyed) {
            timer->stop();
            if (currentLevel < maxLevel) {
                currentLevel++;
                QMessageBox::information(this, "通关", QString("进入第 %1 关").arg(currentLevel));
                resetBricks();
                resetBallAndPaddle();
                timer->start(16);  // 重新开始
            } else {
                QMessageBox::information(this, "胜利", "恭喜你通关所有关卡！");
            }
            return;
        }

        for (int i = 0; i < items.size(); ++i) {
            items[i]->move();

            if (items[i]->isCaughtBy(paddle)) {
                ItemType t = items[i]->getType();
                if (t == ExtraLife) lives++;
                else if (t == EnlargePaddle) paddle->enlarge(); // 需要你在 Paddle 类中添加 enlarge() 函数
                else if (t == SpeedUpBall) ball->speedUp();     // 同样需实现 speedUp()

                delete items[i];
                items.remove(i);
                i--;
            }
            // 超出界面也删除
            else if (items[i]->rect().top() > height()) {
                delete items[i];
                items.remove(i);
                i--;
            }
        }

        // 8. 重绘
        update();

}

void GameWidget::resetBallAndPaddle()
{
    paddle->resetPosition(width() / 2 - 40, height() - 30);
    ball->resetPosition(width() / 2, height() / 2);
}

