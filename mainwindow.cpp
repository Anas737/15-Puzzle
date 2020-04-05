#include "mainwindow.h"
#include "block.h"
#include <QtWidgets>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    gameStarted = false;

    // Create blocks
    QSize currentScreenSize = getScreenSize(); // Screen size
    for(int blockY = 0; blockY < 4; blockY++) {
        for(int blockX = 0; blockX < 4; blockX++) {

            Block *block = new Block(currentScreenSize, blockX, blockY);

            blocks.push_back(block);
        }
    }

    // GUI

    int stretchValue = (currentScreenSize.height() - currentScreenSize.width()) / 3;

    QWidget *widget = new QWidget ();
    setCentralWidget(widget);

    QGridLayout *layout = new QGridLayout();

    QHBoxLayout* topLayout = new QHBoxLayout();

    // Sound Button

    soundRuning = true;

    QPixmap soundEnabledPix = QPixmap(":/icons/speaker.png");
    soundEnabled = QIcon(soundEnabledPix);

    QPixmap soundDisabledPix = QPixmap(":/icons/mute.png");
    soundDisabled = QIcon(soundDisabledPix);

    soundButton = new QPushButton();
    soundButton->setIcon(soundEnabled);
    soundButton->setIconSize(soundDisabledPix.rect().size());

    topLayout->addStretch(1);
    topLayout->addWidget(soundButton);

    layout->addItem(topLayout, 0, 0, 1, 4);

    layout->setRowStretch(1, stretchValue);

    int startRow = 2;

    for(Block *block : blocks) {
        int blockX = block->getPosition().getX();
        int blockY = block->getPosition().getY();


        layout->addWidget(block,
                          blockY + startRow,
                          blockX);

        // Connect signals
        connect(block, SIGNAL(clicked()), this, SLOT(blockClicked()));
    }

    layout->setRowStretch(4 + startRow, stretchValue);

    // Bottom
    runningTime = 0;
    currentScore = 0;

    currentScoreText = new QLabel("[0 coups]", this);
    runningTimeText = new QLabel("[0 s]", this);
    startGameButton = new QPushButton("Commencer !", this);

    gameTimer = new QTimer(this);
    gameTimer->setInterval(1000);

    graphicsTimer = new QTimer(this);
    graphicsTimer->setInterval(150);

    QHBoxLayout* bottomLayout = new QHBoxLayout();

    bottomLayout->addWidget(runningTimeText);
    bottomLayout->addWidget(currentScoreText);
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(startGameButton);

    layout->addItem(bottomLayout, 5 + startRow, 0, 1, 4);
    widget->setLayout(layout);

    // Graphics
    resetGraphics();

    // Sounds
    backgroundMusic = new QMediaPlayer();
    backgroundMusic->setMedia(QUrl("qrc:/sounds/Tetris.mp3"));
    backgroundMusic->setVolume(30);
    backgroundMusic->play();

    succesSound = new QMediaPlayer();
    succesSound->setMedia(QUrl("qrc:/sounds/Click.wav"));

    failSound = new QMediaPlayer();
    failSound->setMedia(QUrl("qrc:/sounds/Failed.mp3"));

    // Connect signals
    connect(startGameButton, SIGNAL(clicked()), this, SLOT(startGameClicked()));
    connect(soundButton, SIGNAL(clicked()), this, SLOT(soundBtnClicked()));

    connect(gameTimer, SIGNAL(timeout()), this, SLOT(runningTimeUpdate()));
    connect(graphicsTimer, SIGNAL(timeout()), this, SLOT(graphicsUpdate()));

    startNewGame();

}

void MainWindow::startNewGame() {

    // Random block
    int randomBlockX = rand() % 4;
    int randomBlockY = rand() % 4;
    int randomBlockIndex = randomBlockX + 4 * randomBlockY;

    // Block text
    QVector<int> possibleValues;
    for(int i = 1; i < 16; i++) {
        possibleValues.push_back(i);
    }

    random_shuffle(possibleValues.begin(), possibleValues.end());


    int textIndex = 0;
    for(Block *block : blocks) {
        int blockX = block->getPosition().getX();
        int blockY = block->getPosition().getY();
        int blockIndex = blockX + 4 * blockY;

        if(blockIndex != randomBlockIndex) {
            block->setActive(true);
            block->setButtonText(QString::number(possibleValues.at(textIndex)));
            textIndex++;

        } else {
            block->setActive(false);
            block->setButtonText("");
        }

        block->setEnabled(gameStarted);
    }

    // Graphics
    runningTime = 0;
    currentScore = 0;
    runningTimeText->setText("[0 s]");
    currentScoreText->setText("[0 coups]");
}

bool MainWindow::gameSolved() {
    for(int blockIndex = 0; blockIndex < static_cast<int>(blocks.size()); blockIndex++) {
        Block* block = blocks[blockIndex];

        if(!block->getActive())
            continue;

        int blockNumber = block->text().toInt();

        if(blockNumber != (blockIndex + 1))
            return false;
    }

    return true;
}

QSize MainWindow::getScreenSize() const {
    QList<QScreen*> screens = QGuiApplication::screens();
    if(screens.size() == 0) {
        throw "Pas d'ecran !";
    }

    return screens[0]->availableSize();
}

// Slots
void MainWindow::blockClicked() {
    if(gameStarted) {

        Block* clickedBlock = dynamic_cast<Block*>(sender());
        QMediaPlayer *currentSound = nullptr;
        QPalette currentPalette = palette();

        if(clickedBlock != nullptr) {
            if(clickedBlock->isMoved(blocks))
            {
                currentScore++;
                QString scoreUpdate = QString("[%1 coups]").arg(currentScore);

                currentScoreText->setText(scoreUpdate);

                if(gameSolved()) {
                    gameStarted = false;
                }

                currentSound = succesSound;

                currentPalette.setColor(QPalette::Window, QColor(Qt::darkGreen));

            } else {
                currentSound = failSound;

                currentPalette.setColor(QPalette::Window, QColor(Qt::red));
            }

            if(soundRuning) {
                if(currentSound->state() == QMediaPlayer::PlayingState) {
                    currentSound->setPosition(0);
                } else if(currentSound->state() == QMediaPlayer::StoppedState) {
                    currentSound->play();
                }
            }

            setPalette(currentPalette);
            graphicsTimer->start();
        }
    }
}

void MainWindow::soundBtnClicked() {

    soundRuning = !soundRuning;
    QIcon currentSoundIcon;

    if(soundRuning) {
        currentSoundIcon = soundEnabled;
    } else {
        currentSoundIcon = soundDisabled;
    }

    QIcon buttonIcon(currentSoundIcon);
    soundButton->setIcon(buttonIcon);

    backgroundMusic->setMuted(!soundRuning);
}

void MainWindow::startGameClicked() {

    if(!gameStarted)
    {
        startGameButton->setText("Nouvelle partie !");
        gameStarted = true;
    }

    gameTimer->start();
    startNewGame();
}

void MainWindow::runningTimeUpdate() {
    runningTime++;
    QString textUpdate = QString("[%1 s]").arg(runningTime);

    runningTimeText->setText(textUpdate);
}

void MainWindow::graphicsUpdate() {

    resetGraphics();
    graphicsTimer->stop();

}

// Graphics
QSize MainWindow::sizeHint() const {

    QSize screenSize = getScreenSize();

    int width, height;
    width = height = screenSize.width();

    QSize windowSize = QSize(width, height);
    return windowSize;
}

void MainWindow::resetGraphics() {

    qDebug() << "resetGraphics";

    QPalette currentPalette = palette();

    currentPalette.setColor(QPalette::Window, QColor(Qt::black));
    currentPalette.setColor(QPalette::WindowText, QColor(Qt::white));

    currentPalette.setColor(QPalette::Button, QColor(240, 230, 140));
    currentPalette.setColor(QPalette::ButtonText, QColor(Qt::black));

    setPalette(currentPalette);

    setFont(QFont("Helvetica [Cronyx]", 15, QFont::Bold));

}

MainWindow::~MainWindow()
{
}

