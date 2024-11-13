#pragma once

#include "primitive.hpp"

#include <vector>

class Integrator {

public:
    Primitive aggregate;

protected:
    Integrator(Primitive aggregate, std::vector<Light> lights) : aggregate(aggregate), lights(lights);

}
