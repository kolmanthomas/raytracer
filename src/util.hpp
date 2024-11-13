#pragma once

#include <Eigen/Dense>

#include <iostream>
#include <limits>
#include <memory>
#include <random>

const double inf  = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline void seed()
{
    srand((unsigned int) time(0));
}

inline Eigen::Vector3d random_unit_vector()
{
    return Eigen::Vector3d::Random().normalized();
}

inline Eigen::Vector3d random_on_hemisphere(const Eigen::Vector3d normal)
{
    Eigen::Vector3d on_unit_sphere = Eigen::Vector3d::Random();

    if (on_unit_sphere.dot(normal) > 0.0) {
        return on_unit_sphere; 
    } else {
        return -on_unit_sphere;
    }
}

inline double random_double()
{
    // Returns a random real in [0,1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min)*random_double();
}

