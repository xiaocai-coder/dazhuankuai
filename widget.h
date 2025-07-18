#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include <QVector>
#include <QPushButton>
#include <QMediaPlayer>
#include "Item.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    GameWidget(QWidget *parent = nullptr);
    void startGame();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
private slots:

    void updateGame();


private:
    QTimer *timer;
    Paddle *paddle;
    Ball *ball;
    QVector<Brick*> bricks;
    bool leftPressed = false;
    bool rightPressed = false;
    void resetBricks();
    int score = 0; // 当前得分
    QFont scoreFont;
    int lives = 3;              // 初始生命值
    void resetBallAndPaddle(); // 重置球和挡板
    QPushButton *restartButton;//重新开始按钮

    bool isPaused = false;//判断是否暂停

    QMediaPlayer *player;

    int currentLevel = 1;             // 当前关卡
    const int maxLevel = 3;           // 总关卡数
     QVector<Item*> items;//道具


};

#endif // GAMEWIDGET_H
