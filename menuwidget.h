#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);
    int selectedLevel() const;

signals:
    void startGameClicked();
    void showHighScoresClicked();

private:
    QPushButton *startButton;
    QPushButton *exitButton;
    QPushButton *highScoreButton;
    QComboBox *levelSelector;
};

#endif // MENUWIDGET_H
