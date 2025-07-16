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

private:
    QPushButton *startButton;
    QPushButton *exitButton;
};

#endif // MENUWIDGET_H
