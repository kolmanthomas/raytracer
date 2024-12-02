#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "src/gpu/gpu.hpp"

#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "interval.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "window.hpp"

#include "src/gpu/instance.hpp"
#include "src/gpu/physical_device.hpp"
#include "src/gpu/logical_device.hpp"
#include "src/gpu/surface.hpp"
#include "src/handy.hpp"

#include <stdexcept>

#include "spdlog/spdlog.h"

using Vector3d = Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

const std::vector<const char*> required_device_extensions = {
    VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void init_window()
{
}

void run()
{
    // World
    
    hittable_list world;

    /*
    auto material_ground = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.50);
    auto material_bubble = std::make_shared<Dielectric>(1.00 / 1.50);
    auto material_right  = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<sphere>(Point3d( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<sphere>(Point3d( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<sphere>(Point3d(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<sphere>(Point3d(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(std::make_shared<sphere>(Point3d( 1.0,    0.0, -1.0),   0.5, material_right));

    Camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov     = 20;
    cam.lookfrom = Point3d(-2, 2, 1);
    cam.lookat   = Point3d(0, 0, -1);
    cam.vup      = Vector3d(0, 1, 0);

    cam.defocus_angle = 10.0;
    cam.focus_dist = 3.4;

    cam.render(world);
    */

    auto ground_material = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(Point3d(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3d center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3d(4, 0.2, 0)).norm() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto rand1 = color::Random();
                    auto rand2 = color::Random();
                    auto albedo = color(rand1[0] * rand2[0], rand1[1] * rand2[1], rand1[2] * rand2[2]);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = (color::Random() + Vector3d(1, 1, 1))/2;
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<sphere>(Point3d(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(Point3d(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(Point3d(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = Point3d(13,2,3);
    cam.lookat   = Point3d(0,0,0);
    cam.vup      = Vector3d(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}

struct VulkanMemoryManagement {
public:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    GLFWwindow* window;
    gpu::physical_device::LightlyDevice physical_device;
    VkDevice device;
    VkQueue graphics_queue;
};
VulkanMemoryManagement vmm; 

/*
*
*/
void cleanup()
{
    gpu::instance::DestroyDebugUtilsMessengerEXT(vmm.instance, vmm.debugMessenger, nullptr);
    //vkDestroySurfaceKHR(vmm.instance, vmm.surface, nullptr);
    vkDestroyInstance(vmm.instance, nullptr);
    glfwDestroyWindow(vmm.window);
    glfwTerminate();
    SPDLOG_INFO("Terminated GLFW");
}


void execute()
{
    SPDLOG_INFO("Initializing engine");
    GLFWwindow* window = init_glfw();

    VulkanMemoryManagement vmm; 
    vmm.instance = gpu::instance::create_instance("Raytracer");
    gpu::instance::setup_debug_messenger(vmm.instance, vmm.debugMessenger);
    vmm.surface = gpu::surface::create_surface(vmm.instance, window);
    vmm.physical_device = gpu::physical_device::pick_physical_device(vmm.instance, vmm.surface, required_device_extensions);
    vmm.device = gpu::logical_device::create_logical_device(vmm.physical_device, required_device_extensions);

    /*
    vkGetDeviceQueue(vmm.device, gpu::physical_device::find_queue_families(vmm.physical_device, { VK_QUEUE_GRAPHICS_BIT }).graphics_family.value(), 0, &vmm.graphics_queue);
    */
    /*
    while (!glfwWindowShouldClose(window.get())) {
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
    */
    // run();
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
    
    // run();
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("[%^%l%$] [source %s] [function %!] [line %#] %v");
    // spdlog::set_pattern("%^[%l]%$");
    // spdlog::set_pattern("[%^---%L---%$]");
    // spdlog::set_pattern("%^[%l]%$ [source %s] [function %!] [line %#] %v");
    // auto console = spdlog::stdout_logger_mt("console");
    // auto console = spdlog::("console");
    // spdlog::set_default_logger(console);
    SPDLOG_TRACE("Hello!");

    SPDLOG_INFO("Initializing engine");
    execute();
    cleanup();

    /*
    while (!glfwWindowShouldClose(window.get())) {
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
    */
    // run();

    return EXIT_SUCCESS;
}



