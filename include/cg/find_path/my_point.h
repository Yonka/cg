#include <gmpxx.h>

#ifndef MY_POINT_H
#define MY_POINT_H
class my_point
{
public:
    double x, y;
    int e0, e1;

    my_point();
    my_point(double x_, double y_);
    my_point(double x_, double y_, int e0_, int e1_);

//access to edge
    int num_edge_1() const;
    int num_edge_2() const;

    bool operator <(const my_point &p);
    bool operator ==(const my_point &p);
};
#endif // MY_POINT_H

