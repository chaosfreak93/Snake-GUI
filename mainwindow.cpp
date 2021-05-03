#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    wallsCheck = new QCheckBox(this);
    wallsCheck->setGeometry(QRect(100, 360, 80, 40));
    wallsCheck->setText("Walls?");
    wallsCheck->setCheckState(walls ? Qt::Checked : Qt::Unchecked);

    connect(wallsCheck, SIGNAL(stateChanged(int)), this, SLOT(setWallStatus(int)));

    start = new QPushButton(this);
    start->setGeometry(QRect(180, 360, 80, 40));
    start->setText("Start");
    start->setDisabled(false);

    connect(start, SIGNAL(clicked()), this, SLOT(start_game()));

    restart = new QPushButton(this);
    restart->setGeometry(QRect(260, 360, 80, 40));
    restart->setText("Restart");
    restart->setDisabled(true);

    connect(restart, SIGNAL(clicked()), this, SLOT(restart_game()));

    stop = new QPushButton(this);
    stop->setGeometry(QRect(340, 360, 80, 40));
    stop->setText("Stop");
    stop->setDisabled(true);

    connect(stop, SIGNAL(clicked()), this, SLOT(stop_game()));

    scoreLabel = new QLabel(this);
    scoreLabel->setGeometry(QRect(260, 0, 80, 40));
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setText("Score: " + scoreString);
}

void MainWindow::showEvent(QShowEvent *event) {
    logic_timer = new QTimer(this);

    connect(logic_timer, SIGNAL(timeout()), this, SLOT(logic()));

    gameOver = false;
    dir = STOP;
    x = this->width() / 2;
    y = this->height() / 2;
    set_fruit_pos();
    set_score(0);
    event->accept();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    //Feld
    painter.drawRect(150, 50, 300, 300);

    //Schlangenkopf
    painter.drawRect(x - 6, y - 6, 12, 12);

    //Frucht
    painter.drawRect(fruit.x() - 4, fruit.y() - 4, 8, 8);

    for (int k = 0; k < nTail; k++) {
        //Schlangenkörper
        painter.drawRect(tail[k].x() - 2, tail[k].y() - 2, 4, 4);
    }

    event->accept();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (!gameOver) {
        if (event->type() == QEvent::KeyRelease && !event->isAutoRepeat()) {
            input(event->key());
        }
    }
}

void MainWindow::setWallStatus(int state) {
    if (state == 0) {
        walls = false;
    } else if (state == 2) {
        walls = true;
    }
}

void MainWindow::start_game() {
    start->setDisabled(true);

    logic_timer->start(25);
    gameOver = false;

    restart->setDisabled(false);
    stop->setDisabled(false);
}

void MainWindow::restart_game() {
    stop_game();
    srand((unsigned int) time(0));
    gameOver = false;
    dir = STOP;
    set_fruit_pos();
    set_score(0);
    x = this->width() / 2;
    y = this->height() / 2;
    nTail = 0;
    start_game();
}

void MainWindow::stop_game() {
    restart->setDisabled(true);
    stop->setDisabled(true);

    logic_timer->stop();

    start->setDisabled(false);
}

void MainWindow::increase_score(int newScore)  {
    score = score + newScore;
    scoreString = QString::number(score);
    scoreLabel->setText("Score: " + scoreString);
}

void MainWindow::decrease_score(int newScore)  {
    score = score - newScore;
    scoreString = QString::number(score);
    scoreLabel->setText("Score: " + scoreString);
}

void MainWindow::set_score(int newScore)  {
    score = newScore;
    scoreString = QString::number(score);
    scoreLabel->setText("Score: " + scoreString);
}

void MainWindow::input(int key)  {
    switch (key) {
    case Qt::Key_W:
        dir = UP;
        break;
    case Qt::Key_A:
        dir = LEFT;
        break;
    case Qt::Key_S:
        dir = DOWN;
        break;
    case Qt::Key_D:
        dir = RIGHT;
        break;
    }
}

void MainWindow::logic()  {
    if (!gameOver) {
        int prevX = tail[0].x();
        int prevY = tail[0].y();
        int prev2X, prev2Y;
        tail[0].setX(x);
        tail[0].setY(y);
        for (int i = 1; i < nTail; i++) {
            prev2X = tail[i].x();
            prev2Y = tail[i].y();
            tail[i].setX(prevX);
            tail[i].setY(prevY);
            prevX = prev2X;
            prevY = prev2Y;
        }
        switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
        }
        if (x > 450 || x < 150 || y > 350 || y < 50) {
            if (walls)
                GameOver();
            else {
                if (x >= 450)
                    x = 150;
                else if (x < 150)
                    x = 450;

                if (y >= 350)
                    y = 50;
                else if (y < 50)
                    y = 350;
            }
        }
        for (int i = 0; i < nTail; i++)
            if(tail[i].x() == x && tail[i].y() == y)
                GameOver();

        if (x >= fruit.x() - 4 && x <= fruit.x() + 4 && y >= fruit.y() - 4 && y <= fruit.y() + 4) {
            increase_score(10);
            set_fruit_pos();
            nTail++;
        }
        repaint();
    }
}

void MainWindow::set_fruit_pos() {
    srand(time(0));
    int fruitX = rand() % 450;
    while (fruitX < 150) {
        fruitX = rand() % 450;
    }
    int fruitY = rand() % 350;
    while (fruitY < 50) {
        fruitY = rand() % 350;
    }
    fruit.setX(fruitX);
    fruit.setY(fruitY);
}

void MainWindow::GameOver() {
    gameOver = true;

    start->setDisabled(true);
    restart->setDisabled(false);
    stop->setDisabled(true);

    scoreLabel->setText("Game Over!!!");
}

