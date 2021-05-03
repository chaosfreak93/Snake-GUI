#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QCheckBox>

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString scoreString = QString::number(score);
    QPushButton* start;
    QPushButton* restart;
    QPushButton* stop;
    QLabel* scoreLabel;
    QTimer* logic_timer;
    QCheckBox* wallsCheck;
    bool walls = false;
    bool gameOver;
    int x, y, score;
    QPoint fruit;
    QPoint tail[1000];
    int nTail = 0;
    enum eDirection { STOP = 0, UP, LEFT, DOWN, RIGHT };
    eDirection dir;
    MainWindow(QWidget *parent = nullptr);

private:
    void increase_score(int newScore);
    void decrease_score(int newScore);
    void set_score(int newScore);
    void set_fruit_pos();
    void GameOver();
    void input(int key);

private slots:
    void setWallStatus(int state);
    void start_game();
    void restart_game();
    void stop_game();
    void logic();

protected:
    void paintEvent(QPaintEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void showEvent(QShowEvent *event);
};
#endif // MAINWINDOW_H
