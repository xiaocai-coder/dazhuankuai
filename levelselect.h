#ifndef LEVELSELECTWIDGET_H
#define LEVELSELECTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVector>

class LevelSelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LevelSelectWidget(QWidget *parent = nullptr);

signals:
    void levelSelected(int level);
    void returnToMenu(); // 可选：返回模式选择界面

private:
    QVector<QPushButton*> levelButtons;
};

#endif // LEVELSELECTWIDGET_H

