#include "widget.h"
#include "menuwidget.h"
#include "highscore.h"
#include "modeselect.h"
#include "levelselect.h"

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuWidget menu;
    ModeSelectWidget modeSelect;
    GameWidget game;
    HighScoreWidget highScore;
    LevelSelectWidget levelSelect;

    menu.setWindowTitle("🧱 超级打砖块：砖力无限 - 主菜单");
    game.setWindowTitle("🧱 超级打砖块：砖力无限 - 游戏中");
    highScore.setWindowTitle("🧱 超级打砖块：砖力无限 - 高分榜");
    modeSelect.setWindowTitle("🧱 超级打砖块：砖力无限 - 模式选择");
    levelSelect.setWindowTitle("🧱 超级打砖块：砖力无限 - 关卡选择");

        menu.show();
        game.hide();
        highScore.hide();
        modeSelect.hide();

        // 点击菜单的“开始游戏” → 显示模式选择界面
        QObject::connect(&menu, &MenuWidget::startGameClicked, [&]() {
            menu.hide();
            modeSelect.show();
        });

        // 模式选择 → 无尽模式
        QObject::connect(&modeSelect, &ModeSelectWidget::endlessModeSelected, [&]() {
            modeSelect.hide();
            game.setMode(GameWidget::Endless);
            game.startGame();  // 默认是无尽模式逻辑
            game.show();
        });

        // 模式选择 → 关卡模式（后续支持关卡系统）
        // 点击模式选择中的“关卡模式”按钮
            QObject::connect(&modeSelect, &ModeSelectWidget::levelModeSelected, [&]() {
                modeSelect.hide();
                levelSelect.show(); // 显示关卡选择界面
            });
        // 模式选择返回主菜单
            QObject::connect(&modeSelect, &ModeSelectWidget::returnToMenu, [&]() {
                modeSelect.hide();
                menu.show();
            });

            // 选择关卡后，启动游戏并传入关卡号
                QObject::connect(&levelSelect, &LevelSelectWidget::levelSelected, [&](int level) {
                    levelSelect.hide();
                    game.setMode(GameWidget::Level);      // 设置模式为关卡
                    game.setLevel(level);                 // 设置当前关卡（下一步要实现）
                    game.show();
                    game.startGame();
                });

                //返回模式选择界面
                QObject::connect(&levelSelect, &LevelSelectWidget::returnToMenu, [&]() {
                    levelSelect.hide();
                    modeSelect.show();
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


//            QObject::connect(&menu, &MenuWidget::startGameClicked, [&]() {
//                int level = menu.selectedLevel();  // 获取选定关卡
//                game.setLevel(level);              // 设置到 GameWidget
//                game.startGame();                  // 开始游戏
//                menu.hide();
//                game.show();
//            });

            QObject::connect(&game, &GameWidget::gameEndedByUser, [&]() {
                game.hide();
                menu.show();
                // 如果需要，可以重置游戏数据
            });


    menu.show();

    return a.exec();
}
