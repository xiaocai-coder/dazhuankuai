#include "modeselect.h"
#include <QFont>

ModeSelectWidget::ModeSelectWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("选择游戏模式", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);

    btnEndless = new QPushButton("无尽模式", this);
    btnEndless->setFixedSize(200, 50);

    btnLevel = new QPushButton("关卡模式", this);
    btnLevel->setFixedSize(200, 50);

    btnBack = new QPushButton("返回主菜单", this);   // 返回按钮
    btnBack->setFixedSize(200, 50);

    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(40);
    layout->addWidget(btnEndless, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(btnLevel, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(btnBack, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(btnEndless, &QPushButton::clicked, this, &ModeSelectWidget::endlessModeSelected);
    connect(btnLevel, &QPushButton::clicked, this, &ModeSelectWidget::levelModeSelected);
    connect(btnBack, &QPushButton::clicked, this, &ModeSelectWidget::returnToMenu);
}
