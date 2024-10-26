#include "color.hpp"
#include "ray.hpp"

#include <iostream>

#include "spdlog/spdlog.h"
#include "Eigen/Dense"

using Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

double hit_sphere(const Point3d& center, double radius, const ray& r) {
    Vector3d oc = center - r.origin();
    auto a = r.direction().squaredNorm();
    auto h = r.direction().dot(oc);
    auto c = oc.squaredNorm() - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(Point3d(0,0,-1), 0.5, r);
    if (t > 0.0) {
        Vector3d N = Vector3d(r.at(t) - Vector3d(0,0,-1)).normalized();
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    Vector3d unit_direction = Vector3d(r.direction()).normalized();
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}



int main(int argc, const char *const argv[]) 
{
    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = Vector3d(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vector3d(viewport_width, 0, 0);
    auto viewport_v = Vector3d(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - Vector3d(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height- j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                 \n";
}

/*
int main(int argc, const char *const argv[])
{
    spdlog::info("Initialized engine");
}
*/



