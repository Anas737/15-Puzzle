#include "point.h"

Point::Point(){

}
Point::Point(int _x, int _y)
{
    x = _x;
    y = _y;
}

int Point::getX() {
    return x;
}

int Point::getY() {
    return y;
}
