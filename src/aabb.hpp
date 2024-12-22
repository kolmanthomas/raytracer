#pragma once

#include "util.h"
#include "interval.hpp"

class AABB {
public:
    Interval x, y, z;

    AABB() {}
    AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {}

    AABB(const Point3d& a, const Point3d& b) 
    {
        x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
    }

    
};
