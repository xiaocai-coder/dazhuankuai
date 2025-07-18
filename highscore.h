#ifndef HIGHSCOREWIDGET_H
#define HIGHSCOREWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QList>

class HighScoreWidget : public QWidget {
    Q_OBJECT
public:
    explicit HighScoreWidget(QWidget *parent = nullptr);

    void loadScores();             // 读取并显示分数
    void addScore(int score);      // 添加新分数并保存

signals:
    void returnMenuClicked();      // 返回主菜单按钮被点击时发出

private:
    QListWidget *scoreList;        // 用于显示高分列表
    QPushButton *backButton;       // 返回按钮

    QList<int> scores;             // 分数存储列表

    void saveScores();             // 保存分数到文件
};

#endif // HIGHSCOREWIDGET_H

