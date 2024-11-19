#include "vulkan/vulkan.hpp"
#include "GLFW/glfw3.h"

// #include "vulkan/vulkan.hpp"
#include "util.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "interval.hpp"
#include "camera.hpp"
#include "material.hpp"

#include <stdexcept>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_TRACE_ON
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/ansicolor_sink.h"

using Vector3d = Eigen::Vector3d;
using Point3d = Eigen::Vector3d;

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

void GLFW_custom_error_callback(int err, const char* description) {
    SPDLOG_ERROR("GLFW error no. {}: \"{}\"", err, description);
}
    

/*
 * Fails if init_glfw is not called
 */
void create_instance(const std::string& application_name)
{
    SPDLOG_INFO("Creating VkApplicationInfo...");
    // vulkan.hpp:
    // VULKAN_HPP_CONSTEXPR ApplicationInfo( const char * pApplicationName_   = {},
    //                                       uint32_t     applicationVersion_ = {},
    //                                       const char * pEngineName_        = {},
    //                                       uint32_t     engineVersion_      = {},
    //                                       uint32_t     apiVersion_         = {},
    //                                       const void * pNext_              = nullptr ) VULKAN_HPP_NOEXCEPT
    // 
    vk::ApplicationInfo app_info {
        application_name.data(),
        VK_MAKE_VERSION(1, 0, 0),
        "No Engine",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_0
    };
    SPDLOG_INFO("Created VkApplicationInfo");

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    SPDLOG_INFO("Creating VkInstance...");
    // 
    // vulkan.hpp:
    //
    // VULKAN_HPP_CONSTEXPR InstanceCreateInfo( VULKAN_HPP_NAMESPACE::InstanceCreateFlags     flags_                   = {},
    //                                          const VULKAN_HPP_NAMESPACE::ApplicationInfo * pApplicationInfo_        = {},
    //                                          uint32_t                                      enabledLayerCount_       = {},
    //                                          const char * const *                          ppEnabledLayerNames_     = {},
    //                                          uint32_t                                      enabledExtensionCount_   = {},
    //                                          const char * const *                          ppEnabledExtensionNames_ = {},
    //                                          const void *                                  pNext_                   = nullptr) VULKAN_HPP_NOEXCEPT

    /*
    vk::InstanceCreateInfo create_info {
        0,
        app_info,
        0,
        nullptr,
        0,
    };
    SPDLOG_INFO("Created VkInstance");
    */
}

// Functor
struct GLFWwindow_deleter {
        void operator()(GLFWwindow* window) 
        {
            glfwDestroyWindow(window);
            SPDLOG_INFO("Deleted GLFWwindow");
        }
};
using GLFWwindow_h = std::unique_ptr<GLFWwindow, GLFWwindow_deleter>;


/*
* MT-Unsafe
*/
GLFWwindow_h init_glfw()
{
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    SPDLOG_INFO("GLFW version {}.{}.{}", major, minor, rev);

    glfwSetErrorCallback(GLFW_custom_error_callback);

    SPDLOG_INFO("Initializing GLFW...");
    if (!glfwInit()) {
        SPDLOG_CRITICAL("Failed to initialize GLFW");
    }
    SPDLOG_INFO("Initialized GLFW");

    /*
    if (!glfwVulkanSupported()) {
        throw std::runtime_error("Ungodly error");
    }
    */

    // No errors can be made when GLFW is initialized and enums are valid
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    SPDLOG_INFO("Passed window hints to GLFW");

    SPDLOG_INFO("Initializing GLFW window...");
    GLFWwindow * window = glfwCreateWindow(640, 480, "Raytracer", NULL, NULL);
    if (!window) {
        SPDLOG_CRITICAL("Failed to create GLFW window.");
        throw std::runtime_error("Failed window creation");
    }
    SPDLOG_INFO("Initialized GLFW window");
    
    return GLFWwindow_h { window };
}

/*
*
*/
void init_vulkan()
{
    create_instance("Hello!");
}

void cleanup()
{
    glfwTerminate();
    SPDLOG_INFO("Terminated GLFW");
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

    SPDLOG_INFO("Initializing engine");
    GLFWwindow_h window = init_glfw();
    init_vulkan();

    while (!glfwWindowShouldClose(window.get())) {
//        glfwSwapBuffers(window.get());
//       glfwPollEvents();
    }
    cleanup();

    return EXIT_SUCCESS;
}



