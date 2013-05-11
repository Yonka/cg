#include "cg/find_path/edge.h"

edge::edge(): a(), b() {}

edge::edge(const my_point &a, const my_point &b): a(a), b(b) {}

my_point edge::another(const my_point &a) const
{
    if (this->first() == a)
        return this->second();
    else
        return this->first();
}

my_point edge::first() const
{
    return a;
}

my_point edge::second() const
{
    return b;
}
