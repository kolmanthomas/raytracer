#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#define INSTRUMENTATION_ON

#include "src/gpu/gpu.hpp"

#include "src/gpu/buffer.hpp"
#include "src/gpu/command_pool.hpp"
#include "src/gpu/descriptor_set.hpp"
#include "src/gpu/fence.hpp"
#include "src/gpu/framebuffer.hpp"
#include "src/gpu/instance.hpp"
#include "src/gpu/physical_device.hpp"
#include "src/gpu/image.hpp"
#include "src/gpu/logical_device.hpp"
#include "src/gpu/semaphore.hpp"
#include "src/gpu/shader.hpp"
#include "src/gpu/renderpass.hpp"
#include "src/gpu/surface.hpp"
#include "src/gpu/swap_chain.hpp"
#include "src/gpu/pipeline.hpp"

#include "glslang/Include/glslang_c_interface.h"

#include "src/circular_buffer.hpp"
#include "src/file.hpp"
#include "src/shader.hpp"
#include "src/transform.hpp"

#include "src/def.hpp"
#include "src/render.hpp"
#include "src/window.hpp"

#include <stdexcept>
#include <numbers>

#include "spdlog/spdlog.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const std::vector<const char*> required_device_extensions = {
    VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<VkDynamicState> dynamic_states = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR
};

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // top left, red 
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // top right, green
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}, // bottom right, blue
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}} // bottom left, white 
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};


void run()
{
    // World
    
    //hittable_list world;

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

    /*
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
    */


    /*
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
    */

    //std::ofstream file("image.ppm");
    //render(file);

    //eval();
}

struct VulkanMemoryManagement {
public:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;
    GLFWwindow* window;
    gpu::LightlyDevice physical_device;
    VkDevice device;
    VkQueue graphics_queue;
    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swap_chain_images;
    std::vector<VkImageView> swap_chain_image_views;
    std::vector<VkShaderModule> shader_modules {};
    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;
    std::vector<VkFramebuffer> swap_chain_framebuffers;
    VkCommandPool command_pool;
    std::vector<VkCommandBuffer> command_buffers;
    VkDescriptorSetLayout descriptor_set_layout;

    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;
    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;

    std::vector<VkBuffer> uniform_buffers;
    std::vector<VkDeviceMemory> uniform_buffers_memory;
    std::vector<void*> uniform_buffers_mapped;
    
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> in_flight_fences;
};
VulkanMemoryManagement vmm; 

UniformBufferObject ubo;

void cleanup()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(vmm.device, vmm.image_available_semaphores[i], nullptr);
        vkDestroySemaphore(vmm.device, vmm.render_finished_semaphores[i], nullptr);
        vkDestroyFence(vmm.device, vmm.in_flight_fences[i], nullptr);
    }
    SPDLOG_INFO("Destroyed semaphores and fences");

    vkDestroyCommandPool(vmm.device, vmm.command_pool, nullptr);
    SPDLOG_INFO("Destroyed command pool");

    for (auto framebuffer : vmm.swap_chain_framebuffers) {
        vkDestroyFramebuffer(vmm.device, framebuffer, nullptr);
    }
    SPDLOG_INFO("Destroyed framebuffers");

    for (auto image_view : vmm.swap_chain_image_views) {
        vkDestroyImageView(vmm.device, image_view, nullptr);
    }
    SPDLOG_INFO("Destroyed image views");

    vkDestroyBuffer(vmm.device, vmm.index_buffer, nullptr);
    vkFreeMemory(vmm.device, vmm.index_buffer_memory, nullptr);

    vkDestroyBuffer(vmm.device, vmm.vertex_buffer, nullptr);
    vkFreeMemory(vmm.device, vmm.vertex_buffer_memory, nullptr);
    SPDLOG_INFO("Destroyed vertex buffer and associated memory");

    vkDestroyRenderPass(vmm.device, vmm.render_pass, nullptr);
    SPDLOG_INFO("Destroyed render pass");

    vkDestroyPipeline(vmm.device, vmm.graphics_pipeline, nullptr);
    SPDLOG_INFO("Destroyed graphics pipeline");

    vkDestroyPipelineLayout(vmm.device, vmm.pipeline_layout, nullptr);
    SPDLOG_INFO("Destroyed pipeline layout");

    vkDestroySwapchainKHR(vmm.device, vmm.swap_chain, nullptr);
    SPDLOG_INFO("Destroyed swap chain");

    vkDestroySurfaceKHR(vmm.instance, vmm.surface, nullptr);
    SPDLOG_INFO("Destroyed Vulkan surface");
    
    for (auto shader_module : vmm.shader_modules) {
        vkDestroyShaderModule(vmm.device, shader_module, nullptr);
    }
    SPDLOG_INFO("Destroyed Vulkan shader modules");

    vkDestroyDevice(vmm.device, nullptr);
    SPDLOG_INFO("Destroyed Vulkan logical device");

    if (enable_validation_layers) {
        gpu::DestroyDebugUtilsMessengerEXT(vmm.instance, vmm.debugMessenger, nullptr);
        SPDLOG_INFO("Destroyed debug utils messenger");
    }

    vkDestroyInstance(vmm.instance, nullptr);
    SPDLOG_INFO("Destroyed Vulkan instance");

    glfwDestroyWindow(vmm.window);
    SPDLOG_INFO("Destroyed GLFW window");

    glfwTerminate();
    SPDLOG_INFO("Terminated GLFW");
}

void update_uniform_buffer(uint32_t current_image)
{
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto current_time = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();

    UniformBufferObject ubo {};
    /*
    ubo.model = Eigen::Matrix4f::Identity() * Eigen::AngleAxisf(time * std::numbers::pi / 4, Eigen::Vector3f::UnitZ());
    ubo.view = transform::world_to_camera(Eigen::Vector3f(2.0f, 2.0f, 2.0f), Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f(0.0f, 0.0f, 1.0f));
    */
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), gpu::swap_chain_extent.width / (float) gpu::swap_chain_extent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(vmm.uniform_buffers_mapped[current_image], &ubo, sizeof(ubo));
}

void draw_frame(uint32_t current_frame)
{
    vkWaitForFences(vmm.device, 1, &vmm.in_flight_fences[current_frame], VK_TRUE, std::numeric_limits<uint64_t>::max()); 
    vkResetFences(vmm.device, 1, &vmm.in_flight_fences[current_frame]);

    uint32_t image_index;
    vkAcquireNextImageKHR(vmm.device, vmm.swap_chain, std::numeric_limits<uint64_t>::max(), vmm.image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(vmm.command_buffers[current_frame], 0);
    gpu::record_command_buffer(vmm.command_buffers[current_frame], vmm.render_pass, vmm.swap_chain_framebuffers, image_index, vmm.graphics_pipeline, vmm.vertex_buffer, vmm.index_buffer, vertices, indices);

    VkSemaphore wait_semaphores[] = {vmm.image_available_semaphores[current_frame]};
    VkSemaphore signal_semaphores[] = { vmm.render_finished_semaphores[current_frame] };
    VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSubmitInfo submit_info {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = wait_semaphores,
        .pWaitDstStageMask = wait_stages,
        .commandBufferCount = 1,
        .pCommandBuffers = &vmm.command_buffers[current_frame],
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signal_semaphores,
    };

    if (vkQueueSubmit(vmm.graphics_queue, 1, &submit_info, vmm.in_flight_fences[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkSwapchainKHR swap_chains[] = { vmm.swap_chain };
    VkPresentInfoKHR present_info {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signal_semaphores,
        .swapchainCount = 1,
        .pSwapchains = swap_chains,
        .pImageIndices = &image_index,
        .pResults = nullptr
    };

    vkQueuePresentKHR(vmm.graphics_queue, &present_info);
}



void execute()
{
    SPDLOG_INFO("Initializing engine");
    GLFWwindow* window = init_glfw();

    vmm.instance = gpu::create_instance("Raytracer");
    gpu::setup_debug_messenger(vmm.instance, vmm.debugMessenger);
    vmm.surface = gpu::create_surface(vmm.instance, window);
    vmm.physical_device = gpu::pick_physical_device(vmm.instance, vmm.surface, required_device_extensions);
    vmm.device = gpu::create_logical_device(vmm.physical_device, required_device_extensions);
    vmm.graphics_queue = gpu::get_graphics_queue(vmm.physical_device, vmm.device);

    vmm.swap_chain = gpu::create_swap_chain(vmm.physical_device.device, vmm.device, vmm.surface, window, 800, 600);
    vmm.swap_chain_images = gpu::get_swap_chain_images(vmm.device, vmm.swap_chain);

    vmm.swap_chain_image_views = gpu::create_image_views(vmm.swap_chain_images, vmm.device);

    // Compile shaders
    
    auto vert_shader_code = read_binary_file("base_vert.spv");
    auto frag_shader_code = read_binary_file("base_frag.spv");

    auto vert_shader = read_text_file("/Users/thomas/programming/raytracer/src/shaders/base_vert.vert");


    /*
    ShaderSource vertex_shader_source {
        .name = "base_vert.vert",
        .source = vert_shader
    };
    compile_shader_to_spirv(vertex_shader_source, GLSLANG_STAGE_VERTEX);
    */

    vmm.shader_modules.push_back(gpu::create_shader_module(vmm.device, vert_shader_code));
    vmm.shader_modules.push_back(gpu::create_shader_module(vmm.device, frag_shader_code));

    vmm.render_pass = gpu::create_render_pass(vmm.device);
    //vmm.descriptor_set_layout = gpu::create_descriptor_set_layout(vmm.device);
    //std::tie(vmm.pipeline_layout, vmm.graphics_pipeline) = gpu::create_pipeline(vmm.device, vmm.shader_modules[0], vmm.shader_modules[1], vmm.render_pass, vmm.descriptor_set_layout);
    std::tie(vmm.pipeline_layout, vmm.graphics_pipeline) = gpu::create_pipeline(vmm.device, vmm.shader_modules[0], vmm.shader_modules[1], vmm.render_pass); 

    vmm.swap_chain_framebuffers = gpu::create_framebuffer(vmm.swap_chain_image_views, vmm.device, vmm.render_pass);

    vmm.command_pool = gpu::create_command_pool(vmm.physical_device, vmm.device);

    auto vertex_buffer = gpu::create_vertex_buffer(vmm.physical_device.device, vmm.device, vertices, vmm.command_pool, vmm.graphics_queue);
    vmm.vertex_buffer = vertex_buffer.data;
    vmm.vertex_buffer_memory = vertex_buffer.memory;

    auto index_buffer = gpu::create_index_buffer(vmm.physical_device.device, vmm.device, indices, vmm.command_pool, vmm.graphics_queue);
    vmm.index_buffer = index_buffer.data;
    vmm.index_buffer_memory = index_buffer.memory;

    /*
    auto ub = gpu::create_uniform_buffer(vmm.physical_device.device, vmm.device); 
    vmm.uniform_buffers = ub.uniform_buffers;
    vmm.uniform_buffers_memory = ub.uniform_buffers_memory;
    vmm.uniform_buffers_mapped = ub.uniform_buffers_mapped;
    */

    vmm.command_buffers = gpu::create_command_buffers(vmm.device, vmm.command_pool);


    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vmm.image_available_semaphores.push_back(gpu::create_semaphore(vmm.device));
        vmm.render_finished_semaphores.push_back(gpu::create_semaphore(vmm.device));
        vmm.in_flight_fences.push_back(gpu::create_fence(vmm.device));
    }

    CircularBuffer<float, 64> framerate_buffer;

    uint32_t current_frame = 0;
    SPDLOG_INFO("Beginning render loop!");
    while (!glfwWindowShouldClose(window)) {
#ifdef INSTRUMENTATION_ON 
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif
        current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

        glfwPollEvents();
        draw_frame(current_frame);

#ifdef INSTRUMENTATION_ON
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        auto elapsed = static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        framerate_buffer.push(elapsed);
        SPDLOG_INFO("Frame time: {}s", framerate_buffer.moving_average().value() / 1000000);
#endif
    }

    vkDeviceWaitIdle(vmm.device);
    // run();
}



// Preconditions for program:
// std::size_t is at least 32 bits
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
    //run();

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



