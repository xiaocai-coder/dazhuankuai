#include "widget.h"
#include "menuwidget.h"
#include "highscore.h"

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuWidget menu;
    GameWidget game;
    HighScoreWidget highScore;

    menu.setWindowTitle("🧱 超级打砖块：砖力无限 - 主菜单");
    game.setWindowTitle("🧱 超级打砖块：砖力无限 - 游戏中");
    highScore.setWindowTitle("🧱 超级打砖块：砖力无限 - 高分榜");



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

                    QString playerName = QInputDialog::getText(nullptr, "游戏结束",
                                            QString("你的得分是 %1\n请输入你的名字：").arg(finalScore),
                                            QLineEdit::Normal, "玩家");


                    if (!playerName.isEmpty()) {
                                highScore.addScore(playerName, finalScore);
                            }

                            // 自动跳转高分榜
                            highScore.loadScores();
                            highScore.show();
            });

    menu.show();

    return a.exec();
}
