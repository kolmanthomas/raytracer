#include "window.hpp"
void init_window()
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }
}
