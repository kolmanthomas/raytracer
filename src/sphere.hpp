#pragma once

#include "def.hpp"

inline bool hit_sphere(const Point3f& center, float radius, const Ray& r)
{
    Vector3f oc = center - r.origin();
    auto a = r.direction().dot(r.direction());
    auto b = -2.0 * r.direction().dot(oc);
    auto c = oc.dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

/*
class Sphere : public hittable {
  public:
    sphere(const Point3d& center, double radius, std::shared_ptr<Material> mat) 
    : center(center), radius(std::fmax(0,radius)), mat(mat) {}

    bool hit(const ray& r, Interval ray_t, hit_record& rec) const override {
        Vector3d oc = center - r.origin();
        auto a = r.direction().squaredNorm();
        auto h = r.direction().dot(oc);
        auto c = oc.squaredNorm() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vector3d outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

  private:
    Point3d center;
    double radius;
    std::shared_ptr<Material> mat;
};
*/

