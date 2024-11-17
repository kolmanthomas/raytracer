#pragma once

#include "hittable.hpp"
#include "color.hpp"
#include "util.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const 
    {
        return false;
    }
};

class Lambertian : public Material {
public:
    Lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
    {
        Eigen::Vector3d scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.isZero(1e-8)) {
            scatter_direction = rec.normal;
        }

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class Metal : public Material {
public:
    Metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        Vector3d reflected = reflect(r_in.direction(), rec.normal);
        reflected = reflected.normalized() + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return scattered.direction().dot(rec.normal) > 0;
    }

private:
    color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override 
    {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

        Vector3d unit_direction = r_in.direction().normalized();
        double cos_theta = std::fmin(-unit_direction.dot(rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = ri * sin_theta > 1.0;
        Vector3d direction;

        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = ray(rec.p, direction);
        return true;
    }
private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine), 5);
    }
};

