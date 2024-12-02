#pragma once

#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"

#include "memory"

//
//
//

struct GLFWwindow_deleter {
        void operator()(GLFWwindow* window) 
        {
            glfwDestroyWindow(window);
            SPDLOG_INFO("Deleted GLFWwindow");
        }
};
using GLFWwindow_h = std::unique_ptr<GLFWwindow, GLFWwindow_deleter>;

GLFWwindow* init_glfw();



