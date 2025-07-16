#include "widget.h"
#include "menuwidget.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MenuWidget menu;
    GameWidget game;

    game.hide();
    // 点击开始游戏时，显示游戏窗口，隐藏菜单
    QObject::connect(&menu, &MenuWidget::startGameClicked, [&]() {
        menu.hide();
        game.show();
        game.startGame();
    });

    menu.show();

    return a.exec();
}
