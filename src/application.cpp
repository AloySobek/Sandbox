#include <GLFW/glfw3.h>

class Window {
  public:
    bool initialized;

    Window(int width, int height, const char *title) : width_{0}, height_{0}, window_{nullptr} {
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

        window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);

        if (!window_) {
            throw std::runtime_error("Could not create GLFW window");
        }

        glfwSetWindowUserPointer(window_, (void *)this);

        glfwMakeContextCurrent(window_);

        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Could not initialize GLEW library");
        }

        glfwSetFramebufferSizeCallback(window_, [](GLFWwindow *window, int width, int height) {
            Window *self = (Window *)glfwGetWindowUserPointer(window);

            self->width_ = width;
            self->height_ = height;

            glViewport(0, 0, self->width_, self->height_);
        });

        glfwGetFramebufferSize(window_, &width_, &height_);

        glViewport(0, 0, width_, height_);

        glfwSetKeyCallback(window_,
                           [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                               if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
                                   glfwSetWindowShouldClose(window, true);
                               }
                           });
    }

    GLFWwindow *get_window() { return window_; }

    int get_width() { return width_; }
    int get_height() { return height_; }

    ~Window() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

  private:
    GLFWwindow *window_;
    int width_, height_;
};

class Application {
  public:
    Application() {}

    Application(const Application &other) = delete;
    Application &operator=(const Application &other) = delete;

    void loop() {}

    ~Application() {}

  private:
};
