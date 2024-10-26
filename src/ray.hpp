#ifndef RAY_HPP
#define RAY_HPP

#include "Eigen/Dense"

using Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

class ray {
  public:
    ray() {}

    ray(const Point3d& origin, const Vector3d& direction) : orig(origin), dir(direction) {}

    const Point3d& origin() const  { return orig; }
    const Vector3d& direction() const { return dir; }

    Point3d at(double t) const {
        return orig + t*dir;
    }

  private:
    Point3d orig;
    Vector3d dir;
};

#endif
