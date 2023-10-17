#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

class Window {
   public:
    int width_, height_;
    GLFWwindow *window_;

    Window(int width, int height, const char *title)
        : width_{width}, height_{height}, window_{nullptr} {
        if (!glfwInit()) {
            throw std::runtime_error("Could not initialize GLFW library");
        }

        glfwSetErrorCallback([](int code, const char *description) {
            spdlog::error("GLFW error occurred. Code: {}; Description: {}", code, description);
        });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

        window_ = glfwCreateWindow(width_, height_, title, nullptr, nullptr);

        if (!window_) {
            throw std::runtime_error("Could not create GLFW window");
        }

        glfwMakeContextCurrent(window_);

        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Could not initialize GLEW library");
        }

        glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            glViewport(0, 0, width, height);
        });

        glViewport(0, 0, width_ * 2, height_ * 2);
    }

    int GetWidth() { return width_; }

    int GetHeight() { return height_; }

    ~Window() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }
};

#endif