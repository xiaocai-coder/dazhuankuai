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

    paddleTimer = new QTimer(this);
    connect(paddleTimer, &QTimer::timeout, this, [=]() {
        if (isPaddleEnlarged) {
            paddle->resetWidth(); // 恢复原始宽度
            isPaddleEnlarged = false;
            paddleTimer->stop();
        }
    });
    endGameButton = new QPushButton("结束游戏", this);
    endGameButton->setGeometry(width()/2 - 50, height()/2 + 40, 100, 40);
    endGameButton->hide();

    connect(endGameButton, &QPushButton::clicked, this, [=]() {
        timer->stop();      // 停止游戏定时器
        emit gameEndedByUser();  // 自定义信号，通知外部
    });


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
            else if (currentLevel == 4) {
            // 第4关：中心强化砖（玩家更难控制反弹）
            if (i >= 1 && i <= 3 && j >= 3 && j <= 6)
                 hits = 3;  // 中间大块强化砖
             else if (i == 2 || j == 4 || j == 5)
                 hits = 2;
              else
                   hits = 1;
                }
              else if (currentLevel == 5) {
                // 第5关：棋盘格布局（间隔强化砖）
               hits = ((i + j) % 2 == 0) ? 3 : 1;
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
        if (isPaused)
            resumeGame();
        else
            pauseGame();
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
    isPaused =false;

    leftPressed = false;
    rightPressed = false;

    if (endGameButton)
            endGameButton->hide();


    qsrand(QTime::currentTime().msec());

    if (currentMode == Endless) {
        currentLevel = 1;
        resetBricks();  // 默认模式下加载固定砖块布局
    } else if (currentMode == Level) {
        //loadLevel(currentLevel);  // 加载指定关卡砖块布局
        resetBricks();
    }

    paddle->resetPosition(width() / 2 - 40, height() - 30);
    ball->resetPosition(width() / 2, height() / 2);


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






    // 6. 检测球是否掉到底部（后续可加 Game Over 逻辑）
    if (ball->rect().bottom() >= height()) {
        lives--;
        if (lives <= 0) {
            timer->stop();
            QMessageBox::information(this, "Game Over", "你输了！");

            emit gameOver(score);

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

                emit gameOver(score);

            }
            return;
        }

        for (int i = 0; i < items.size(); ++i) {
            Item *item = items[i];
            item->move();
            if (item->isCaughtBy(paddle)) {
                switch (item->getType()) {
                case EnlargePaddle:
                    if (!isPaddleEnlarged) {
                        paddle->enlarge();
                        isPaddleEnlarged = true;
                        paddleTimer->start(5000);  // 5秒后恢复
                    }
                    break;
                case ExtraLife:
                    lives++;
                    break;
                case SpeedUpBall:
                    ball->speedUp();
                    break;
                }
                delete item;
                items.removeAt(i);
                --i;
            }
        }


        // 8. 重绘
        update();

}

void GameWidget::resetBallAndPaddle()//重启
{
    paddle->resetPosition(width() / 2 - 40, height() - 30);
    ball->resetPosition(width() / 2, height() / 2);
}

void GameWidget::setMode(GameMode mode) {
    currentMode = mode;
    if (mode == Endless) {
            currentLevel = 1;   // 无尽模式从第一关开始
    }
}

void GameWidget::setLevel(int level) {
    currentLevel = level;
}

void GameWidget::loadLevel(int level)
{
    // 清除旧砖块
    qDeleteAll(bricks);
    bricks.clear();

    int rows = 5 + level;  // 不同关卡行数不同
    int cols = 10;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x = 60 + j * 50;
            int y = 50 + i * 30;
            Brick *brick = new Brick(x, y, 40);
            bricks.append(brick);
        }
    }
}


void GameWidget::pauseGame()
{
    if (isPaused) return;
    isPaused = true;
    timer->stop();
    endGameButton->show();
    update();
}

void GameWidget::resumeGame()
{
    if (!isPaused) return;
    isPaused = false;
    timer->start(16);
    endGameButton->hide();
    update();
}

