#pragma once

#include <Eigen/Dense>

#include <iostream>
#include <limits>
#include <memory>
#include <random>

using Eigen::Vector3d;
using Point3f = Eigen::Vector3f;

const double inf  = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline void seed()
{
    srand((unsigned int) time(0));
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

inline Eigen::Vector3d random_unit_vector()
{
    return Eigen::Vector3d::Random().normalized();
}

inline Vector3d random_in_unit_disk()
{
    while (true) {
        Vector3d p = Vector3d(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.squaredNorm() < 1) {
            return p;
        }
    }
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

inline Vector3d refract(const Vector3d& uv, const Vector3d n, double etai_over_etat)
{
    auto cos_theta = std::fmin(n.dot(-uv), 1.0);
    Vector3d r_out_perp = etai_over_etat * (uv + cos_theta*n);
    Vector3d r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.squaredNorm())) * n;
    return r_out_perp + r_out_parallel;
}

inline Eigen::Vector3d reflect(const Eigen::Vector3d v, const Eigen::Vector3d& n)
{
    return v - 2*v.dot(n)*n;
}
