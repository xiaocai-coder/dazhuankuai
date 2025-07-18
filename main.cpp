#include "widget.h"
#include "menuwidget.h"
#include "highscore.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuWidget menu;
    GameWidget game;
    HighScoreWidget highScore;


    game.hide();
    highScore.hide();
    // 点击开始游戏时，显示游戏窗口，隐藏菜单
    QObject::connect(&menu, &MenuWidget::startGameClicked, [&]() {
        menu.hide();
        game.show();
        game.startGame();
    });


    // 点击高分榜按钮，隐藏菜单，显示高分榜
        QObject::connect(&menu, &MenuWidget::showHighScoresClicked, [&]() {
            menu.hide();
            highScore.show();
        });
    // 高分榜返回按钮：返回主菜单
            QObject::connect(&highScore, &HighScoreWidget::returnMenuClicked, [&]() {
                highScore.hide();
                menu.show();
            });

            QObject::connect(&game, &GameWidget::gameOver, [&](int finalScore) {
                game.hide();
                highScore.addScore(finalScore);
                highScore.loadScores();
                highScore.show();
            });

    menu.show();

    return a.exec();
}
