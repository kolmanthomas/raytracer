#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "interval.hpp"
#include "camera.hpp"

#include "spdlog/spdlog.h"

using Vector3d = Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

void run()
{
    // World
    
    hittable_list world;
    world.add(std::make_shared<sphere>(Point3d(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(Point3d(0, -100.5, -1), 100));

    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.render(world);
}

int main(int argc, const char *const argv[])
{
    // <Convert command-line argiments to vector of strings>
    // <Declare variables for parsed command line>
    // <Process command-line arguments>
    // <Initialize pbrt>
    // <Parse provided scene description files>
    // <Render the secene>
    // <Clean up after rendering the scene>
    run();
}



