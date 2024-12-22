#pragma once

#include <spdlog/spdlog.h>

#include "src/def.hpp"
#include "src/color.hpp"

#include <fstream>



/*
inline static void render(std::ofstream& file)
{
    SPDLOG_TRACE("Rendering..."); 

    file << "P3\n" << global::image_width << " " << global::image_height << "\n255\n";

    for (int j = 0; j < global::image_height; j++) {
        SPDLOG_TRACE("\rScanlines remaining: {}", global::image_height - j);
        for (int i = 0; i < global::image_width; i+8) {
            auto pixel_center = global::pixel00_loc + (i * global::pixel_delta_u) + (j * global::pixel_delta_v);
            auto ray_direction = pixel_center - global::camera_center;
            Ray ray {.origin=global::camera_center, .direction=ray_direction};

            Vec3 unit_direction = unit_vector(ray.direction);
            auto a = 0.5*(unit_direction.y + 1.0);
            Color pixel_color = (1.0-a)*Color {.x=1.0, .y=1.0, .z=1.0} + a*Color{.x=0.5, .y=0.7, .z=1.0};

            auto r = pixel_color.x;
            auto g = pixel_color.y;
            auto b = pixel_color.z;

        //    r = linear_to_gamma(r);
        //   g = linear_to_gamma(g);
        //   b = linear_to_gamma(b);

        // Translate the [0,1] component values to the byte range [0,255].
        //    static const Interval intensity(0.000, 0.999);
        //    int rbyte = int(255.999 * intensity.clamp(r));
        //    int gbyte = int(255.999 * intensity.clamp(g));
        //    int bbyte = int(255.999 * intensity.clamp(b));

            int rbyte = int(255.999 * r);
            int gbyte = int(255.999 * g);
            int bbyte = int(255.999 * b);
            // Write out the pixel color components.
            file << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';

        }
    }
}
*/
