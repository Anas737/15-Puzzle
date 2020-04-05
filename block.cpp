#include "block.h"

Block::Block(QSize currentScreenSize, int x, int y) : QPushButton()
{
    screenSize = currentScreenSize;
    blockNumber = 4;

    position = Point(x, y);

    // Neighbours on the right / left
    for(int neighbourX = x - 1; neighbourX < x + 2; neighbourX += 2) {

        if(neighbourX < 0 || neighbourX > 3)
            continue;

        Point neighbour = Point(neighbourX, y);

        if(neighbourX - x == 1) // Right
        {
            neighbourPoints.insert(Right, neighbour);
        } else { // Left
            neighbourPoints.insert(Left, neighbour);
        }
    }

    // Neighbours on top / bottom
    for(int neighbourY = y - 1; neighbourY < y + 2; neighbourY += 2) {

        if(neighbourY < 0 || neighbourY > 3)
            continue;

        Point neighbour = Point(x, neighbourY);

        if(neighbourY - y == 1) // Bottom
        {
            neighbourPoints.insert(Bottom, neighbour);
        } else { // Top
            neighbourPoints.insert(Top, neighbour);
        }
    }

    QSize maximumSize = QSize(200, 200);
    setMaximumSize(maximumSize);

    setActive(false);
}

void Block::MoveTo(Block* neighbour) {

    QString currentText = text();

    neighbour->setButtonText(currentText);
    neighbour->setActive(true);

    setText("");
    setActive(false);
}

bool Block::isMoved(QVector<Block*> blocks) {

    if (getActive()) {
        //qDebug() << QString::number(clickedBlock->getPosition().getX()) + "," + QString::number(clickedBlock->getPosition().getY());

        Block* emptyNeighbour = getEmptyNeighbour(blocks);

        if(emptyNeighbour == nullptr) {
            Direction directions[] = {Left, Top, Right, Bottom};

            for(Direction direction : directions) {

                qDebug() << direction;
                Block* currentNeighbour = getNeighbourBlockAtDirection(blocks, direction);

                if(currentNeighbour != nullptr) {
                    if(currentNeighbour->isPushed(blocks, direction)) {
                        emptyNeighbour = currentNeighbour;
                        break;
                    }
                }
            }
        }

        if(emptyNeighbour != nullptr) {
            MoveTo(emptyNeighbour);

            return true;
        }

    }

    return false;
}

bool Block::isPushed(QVector<Block*> blocks, Direction direction) {

    Block* neighbourBlock = getNeighbourBlockAtDirection(blocks, direction);

    if(neighbourBlock != nullptr)
    {
        if (!neighbourBlock->getActive() || neighbourBlock ->isPushed(blocks, direction)) {

            MoveTo(neighbourBlock);

            return true;
        }
    }

    return false;
}

Block* Block::getNeighbourBlockAtDirection(QVector<Block*> blocks, Direction direction) {

    if (neighbourPoints.contains(direction))
    {
        Point point = neighbourPoints[direction];
        int blockIndex = getIndexByCoord(point.getX(), point.getY());

        Block* neighbourBlock = blocks.at(blockIndex);

        return neighbourBlock;
    }

    return nullptr;
}

Block* Block::getEmptyNeighbour(QVector<Block*> blocks) {

    QList<Point> blockNeighbours = neighbourPoints.values();

    for(Point neighbourPosition : blockNeighbours) {

        int blockIndex = getIndexByCoord(neighbourPosition.getX(), neighbourPosition.getY());

        Block* neighbourBlock = blocks.at(blockIndex);

        if (!neighbourBlock->getActive()) {
            return neighbourBlock;
        }
    }

    return nullptr;
}

int Block::getIndexByCoord(int x, int y) {
    int blockIndex = x + 4 * y;

    return blockIndex;
}

void Block::setButtonText(QString text) {
    setText(text);
}

void Block::setActive(bool isActive) {
    active = isActive;
    setVisible(active);
}

Point Block::getPosition() {
    return position;
}

bool Block::getActive() {
    return active;
}

// Graphics
QSize Block::sizeHint() const {

    // Blocksize;

    int width, height;
    width = height = screenSize.width() / 4;

    QSize blockSize = QSize(width, height);
    return blockSize;
}

