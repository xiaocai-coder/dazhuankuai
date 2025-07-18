#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void startGameClicked();
    void showHighScoresClicked();

private:
    QPushButton *startButton;
    QPushButton *exitButton;
    QPushButton *highScoreButton;
};

#endif // MENUWIDGET_H
