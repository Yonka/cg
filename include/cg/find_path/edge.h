#include "my_point.h"

#ifndef EDGE_H
#define EDGE_H
class edge
{
public:
    my_point a, b;
    edge();
    edge(const my_point &a, const my_point &b);

    my_point another(const my_point & a) const;
    my_point first() const;
    my_point second() const;
};

#endif // EDGE_H
