#pragma once

#include "util.hpp"

class Interval {
public:
    double min, max;
    Interval();
    Interval(double min, double max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }
     
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    Interval expand(double delta)
    {
        auto padding = delta/2;
        return Interval(min - padding, max + padding);
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const Interval empty, universe;
};




const Interval Interval::empty = Interval(+inf, -inf);
const Interval Interval::universe = Interval(-inf, +inf);
