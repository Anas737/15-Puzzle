#ifndef POINT_H
#define POINT_H


class Point
{
    public:
        Point();
        Point(int, int);

        int getX();
        int getY();

    private:
        int x;
        int y;
};

#endif // POINT_H
