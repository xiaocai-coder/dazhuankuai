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
    enum GameMode {
            Endless,  // 无尽模式
            Level     // 关卡模式
        };

    void setMode(GameMode mode);   // 设置游戏模式
    void setLevel(int level);      // 设置关卡
    void startGame();

    void pauseGame();
    void resumeGame();



signals:
    void gameOver(int finalScore);
    void gameEndedByUser();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
private slots:

    void updateGame();


private:
    QTimer *timer;
    QTimer *paddleTimer;       // 用于定时恢复挡板长度
    bool isPaddleEnlarged = false;  // 当前是否处于变长状态

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

    GameMode currentMode = Endless;  // 默认是无尽模式
    void loadLevel(int level);

    QPushButton *endGameButton;//结束游戏



};

#endif // GAMEWIDGET_H
