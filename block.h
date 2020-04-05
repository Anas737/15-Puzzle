#ifndef BLOCK_H
#define BLOCK_H
#include <QMap>
#include <QPushButton>
#include "point.h"
#include <QDebug>

class Block : public QPushButton
{
    enum Direction {Left, Top, Right, Bottom} ;
    public:
        Block(QSize, int, int);

        void MoveTo(Block*) ;

        bool isMoved(QVector<Block*>);
        bool isPushed(QVector<Block*>, Direction);

        void setButtonText(QString);
        void setActive(bool);

        Point getPosition();
        bool getActive();

        Block* getNeighbourBlockAtDirection(QVector<Block*>, Direction);
        Block* getEmptyNeighbour(QVector<Block*>);
        int getIndexByCoord(int, int);

        // Graphics
        QSize sizeHint() const;

    private:
        Point position;
        QMap<Direction, Point> neighbourPoints;

        int blockNumber;
        bool active;
        QSize screenSize;

};

#endif // BLOCK_H
