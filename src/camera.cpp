#include "camera.hpp"


Color Camera::ray_color(const Ray& r)
{
    return Color(0, 0, 0);
}
    
void Camera::render()
{
    // Render file
    std::ofstream file("image.ppm");

    if (!file) {
        throw std::runtime_error("Could not open file for writing.");
    }
    file << "P3\n" << m_image_width << " " << m_image_height << "\n255\n";

    for (int j = 0; j < m_image_height; j++) {
        std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
        for (int i = 0; i < m_image_width; i++) {
            auto pixel_center = m_pixel00_loc + (i * m_pixel_delta_u) + (j * m_pixel_delta_v);
            auto ray_direction = pixel_center - m_center;

            Ray r { m_center, ray_direction };

            Color pixel_color = ray_color(r);
            write_color(file, pixel_color);
        }
    }

    std::clog << "\rDone.                   \n";
}
