#include "window.hpp"

// Callback function - do not call directly.
//
// @note This function is called from C code, therefore exceptions cannot be propogated across the ABI boundary. Hence all GLFW calls should be checked for errors at the site of the call as well, this only prints the GLFW error message.
void GLFW_custom_error_callback(int err, const char* description) {
    SPDLOG_ERROR("GLFW error no. {}: \"{}\"", err, description);
}
 
/*
* @thread MT-Unsafe
*/
GLFWwindow* init_glfw()
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

    SPDLOG_INFO("Checking for Vulkan support from GLFW");
    if (!glfwVulkanSupported()) {
        SPDLOG_WARN("This instance of GLFW does not support Vulkan!");
    } else {
        SPDLOG_INFO("Vulkan is supported by GLFW");
    }

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
    
    return window;
}


