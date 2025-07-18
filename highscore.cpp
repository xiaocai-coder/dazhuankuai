
#include "highscore.h"
#include <QSettings>
#include <algorithm>
#include <QFont>
#include <QLabel>

HighScoreWidget::HighScoreWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(400, 500);
    setWindowTitle("高分榜");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("🏆 高分排行榜", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    scoreList = new QListWidget(this);
    scoreList->setFont(QFont("Consolas", 14));
    scoreList->setFocusPolicy(Qt::NoFocus);

    backButton = new QPushButton("返回主菜单", this);
    backButton->setFixedSize(150, 40);
    backButton->setFont(QFont("Arial", 12));

    layout->addWidget(titleLabel);
    layout->addSpacing(10);
    layout->addWidget(scoreList);
    layout->addSpacing(15);
    layout->addWidget(backButton, 0, Qt::AlignCenter);

    connect(backButton, &QPushButton::clicked, this, &HighScoreWidget::returnMenuClicked);

    loadScores();
}

void HighScoreWidget::loadScores()
{
    scores.clear();
    scoreList->clear();

    QSettings settings("MyCompany", "BrickBreakerGame");
    int count = settings.beginReadArray("HighScores");
    for (int i = 0; i < count; ++i) {
        settings.setArrayIndex(i);
        QString name = settings.value("name", "未知玩家").toString();
        int score = settings.value("score", 0).toInt();
        if (score >= 0) {
            scores.append(qMakePair(name, score));
        }
    }
    settings.endArray();

    // 按照分数降序排序
    std::sort(scores.begin(), scores.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        return a.second > b.second;
    });

    // 显示前10名
    for (int i = 0; i < scores.size() && i < 10; ++i) {
        scoreList->addItem(QString("%1. %2 - %3 分").arg(i + 1).arg(scores[i].first).arg(scores[i].second));
    }
}

void HighScoreWidget::addScore(const QString &name, int score)
{
    if (score < 0 || name.isEmpty()) return;

    scores.append(qMakePair(name, score));
    std::sort(scores.begin(), scores.end(), [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
        return a.second > b.second;
    });

    if (scores.size() > 10)
        scores = scores.mid(0, 10); // 仅保留前10名

    saveScores();
    loadScores(); // 重新加载以更新界面
}

void HighScoreWidget::saveScores()
{
    QSettings settings("MyCompany", "BrickBreakerGame");
    settings.beginWriteArray("HighScores");
    for (int i = 0; i < scores.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("name", scores[i].first);
        settings.setValue("score", scores[i].second);
    }
    settings.endArray();
}
