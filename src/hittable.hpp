#pragma once

#include "util.hpp"
#include "ray.hpp"
#include "interval.hpp"

using Vector3d = Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

class Material;

class hit_record {
  public:
    Point3d p;
    Vector3d normal;
    std::shared_ptr<Material> mat;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const Vector3d& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, Interval ray_t, hit_record& rec) const = 0;
};

