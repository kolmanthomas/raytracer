#pragma once

#include "spdlog/spdlog.h"

#include "vec3.hpp"
#include "color.hpp"

#include <iostream>

class Camera {
public:
    Camera(int image_width = 400, float aspect_ratio = 16.0/9.0, float focal_length = 1.0, float viewport_height = 2.0); 

    void render();
private:
    const float m_aspect_ratio; 
    int m_image_width; 
    int m_image_height;
    float m_focal_length;
    float m_viewport_height;
    float m_viewport_width;

    Point3 m_center = Point3 { 0, 0, 0 };

    Vec3 m_viewport_u, m_viewport_v;
    Vec3 m_pixel_delta_u, m_pixel_delta_v;

    Point3 m_viewport_upper_left;

    Point3 m_pixel00_loc;


    Color ray_color(const Ray& r);
};

/*
class Camera {
public:
    float aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    float viewport_width;
    double viewport_height = 2.0;

    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    Point3d lookfrom = Point3d(0, 0, 0);
    Point3d lookat = Point3d(0, 0, -1);
    Vector3d vup = Vector3d(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable& world)
    {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";

    }

    Camera(double aspect_ratio = 16.0 / 9.0, int image_width = 400, int samples_per_pixel = 10, int max_depth = 10, double vfov = 90, Point3d lookfrom = Point3d(0, 0, 0), Point3d lookat = Point3d(0, 0, -1), Vector3d vup = Vector3d(0, 1, 0), double defocus_angle = 0, double focus_dist = 10) : aspect_ratio(aspect_ratio), image_width(image_width), samples_per_pixel(samples_per_pixel), max_depth(max_depth),
               vfov(vfov), lookfrom(lookfrom), lookat(lookat), vup(vup), defocus_angle(defocus_angle), focus_dist(focus_dist) {}

private:
    int    image_height;   // Rendered image height
    double pixel_samples_scale;
    Point3d center;         // Camera center
    Point3d pixel00_loc;    // Location of pixel 0, 0
    Vector3d pixel_delta_u;  // Offset to pixel to the right
    Vector3d pixel_delta_v;  // Offset to pixel below
    Vector3d u, v, w; 
    Vector3d defocus_disk_u;
    Vector3d defocus_disk_v;
    
    void initialize()
    {
        SPDLOG_INFO("Initializing camera");

        // Calculate image height, ensure it is at least 1.
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        // Viewport widths less than one are okay since they are real valued.
        auto viewport_width = viewport_height * (double(image_width)/image_height);


        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        // auto focal_length = (lookfrom - lookat).norm();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        w = (lookfrom - lookat).normalized();
        u = vup.cross(w).normalized();
        v = w.cross(u);


        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        Vector3d viewport_u = viewport_width * u;
        Vector3d viewport_v = viewport_height * -v;

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);

    }

    Vector3d sample_square() const {
        return Vector3d(random_double() - 0.5, random_double() - 0.5, 0);
    }

    Point3d defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r) const {
        return color(0, 0, 0);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;
        if (world.hit(r, Interval(0.001, inf), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                color result = ray_color(scattered, depth - 1, world);
                return color(attenuation[0] * result[0], attenuation[1] * result[1], attenuation[2] * result[2]);
            }
            return color(0, 0, 0);
        }

        Vector3d unit_direction = r.direction().normalized();
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);

    }
};
*/
