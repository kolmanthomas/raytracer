#pragma once

#include <spdlog/spdlog.h>

#include "vec3.hpp"

namespace global {
// Camera(int image_width = 400, float aspect_ratio = 16.0/9.0, float focal_length = 1.0, float viewport_height = 2.0); 
constexpr int image_width = 400;
constexpr float aspect_ratio = 16.0/9.0;
constexpr float focal_length = 1.0;
constexpr int image_height { static_cast<int>(image_width / aspect_ratio) };
constexpr Point3 camera_center { 0, 0, 0 };
constexpr float viewport_height = 2.0;
constexpr float viewport_width = viewport_height * (static_cast<float>(image_width) / image_height);
constexpr Vec3 viewport_u { viewport_width, 0, 0 };
constexpr Vec3 viewport_v { 0, -viewport_height, 0 };
constexpr Vec3 pixel_delta_u { viewport_u / image_width };
constexpr Vec3 pixel_delta_v { viewport_v / image_height };
constexpr Vec3 viewport_upper_left { camera_center - viewport_u / 2 + viewport_v / 2 - Vec3 { .x = 0, .y = 0, .z = focal_length } };
constexpr Point3 pixel00_loc { viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v) };


/*
consteval init()
{
    image_height = static_cast<int>(image_width / aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;

    m_viewport_width = viewport_height * (static_cast<float>(image_width) / m_image_height);

    m_viewport_u = Vector3f { m_viewport_width, 0, 0 };
    m_viewport_v = Vector3f { 0, -m_viewport_height, 0 };

    m_pixel_delta_u = m_viewport_u / m_image_width;
    m_pixel_delta_v = m_viewport_v / m_image_height;
    
    m_viewport_upper_left = m_center - m_viewport_u / 2 + m_viewport_v / 2 - Vector3f (0, 0, m_focal_length);

    m_pixel00_loc = m_viewport_upper_left + 0.5 * (m_pixel_delta_u + m_pixel_delta_v);

    };
}

*/
}

inline void eval()
{
    SPDLOG_INFO("image_height: {}", global::image_height); 
    Vec3 v1 { .x = 1, .y = 2, .z = 3 };
    Vec3 v2 { .x = 2, .y = 4, .z = 6 };


    Vec3 v = v2 / v1;
    
    
}

