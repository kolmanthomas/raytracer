#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "ray.hpp"
#include "color.hpp"
#include "util.hpp"

using Vector3d = Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

class sphere : public hittable {
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

#endif
