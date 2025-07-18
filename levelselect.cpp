#include "levelselect.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

LevelSelectWidget::LevelSelectWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(600, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("选择关卡", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");
    mainLayout->addWidget(title);

    // 水平布局分多排
    QGridLayout *grid = new QGridLayout;
    int totalLevels = 5; // 关卡数量（可自定义）

    for (int i = 0; i < totalLevels; ++i) {
        QPushButton *btn = new QPushButton(QString("第 %1 关").arg(i + 1), this);
        btn->setFixedSize(120, 40);
        grid->addWidget(btn, i / 3, i % 3); // 三列布局

        connect(btn, &QPushButton::clicked, this, [=]() {
            emit levelSelected(i + 1); // 从1开始编号
        });

        levelButtons.append(btn);
    }

    mainLayout->addLayout(grid);

    QPushButton *backButton = new QPushButton("返回", this);
    connect(backButton, &QPushButton::clicked, this, [=]() {
        emit returnToMenu();
    });
    mainLayout->addWidget(backButton, 0, Qt::AlignCenter);
}

