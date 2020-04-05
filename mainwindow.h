#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMediaPlayer>

#include "block.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        // Graphics
        QSize sizeHint() const;
    private:
        bool gameStarted;
        bool soundRuning;

        int runningTime;
        int currentScore;

        QLabel* currentScoreText;
        QLabel* runningTimeText;

        QTimer* gameTimer;
        QTimer* graphicsTimer;

        QPushButton* startGameButton;
        QPushButton* soundButton;

        QVector<Block*> blocks;

        QIcon soundEnabled;
        QIcon soundDisabled;

        QMediaPlayer *backgroundMusic;
        QMediaPlayer *succesSound;
        QMediaPlayer *failSound;

        void startNewGame();
        bool gameSolved();
        QSize getScreenSize() const;

        void resetGraphics();

    private slots:
        void blockClicked();
        void startGameClicked();
        void soundBtnClicked();
        void runningTimeUpdate();
        void graphicsUpdate();
};
#endif // MAINWINDOW_H
