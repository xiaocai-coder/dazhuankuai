#include "menuwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("打砖块小游戏", this);
    QFont titleFont("Arial", 32, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    startButton = new QPushButton("开始游戏", this);
    startButton->setFixedSize(200, 50);

    exitButton = new QPushButton("退出游戏", this);
    exitButton->setFixedSize(200, 50);

    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(50);
    layout->addWidget(startButton, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(exitButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(startButton, &QPushButton::clicked, this, [=]() {
        emit startGameClicked();
    });
    connect(exitButton, &QPushButton::clicked, this, [=]() {
        close();
    });
}
