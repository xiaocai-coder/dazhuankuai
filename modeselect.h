#ifndef MODESELECTWIDGET_H
#define MODESELECTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class ModeSelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModeSelectWidget(QWidget *parent = nullptr);

signals:
    void endlessModeSelected();  // 选择无尽模式
    void levelModeSelected();    // 选择关卡模式
    void returnToMenu();

private:
    QPushButton *btnEndless;
    QPushButton *btnLevel;
    QPushButton *btnBack;
};

#endif // MODESELECTWIDGET_H
