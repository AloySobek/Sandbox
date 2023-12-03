#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>

class ResourceManager {
  public:
    static const GLchar *read_shader_src(const char *filename) {
        GLchar *data = NULL;

        std::ifstream file;
        size_t filesize;

        file.open(filename, std::ifstream::binary | std::ifstream::ate);

        if (!file.good()) {
            std::cout << "Can't open file " << filename << std::endl;
            return NULL;
        }

        filesize = file.tellg();
        file.close();

        data = new GLchar[filesize + 1];

        file.open(filename);
        file.read(data, filesize);

        data[filesize] = '\0';

        return data;
    }
};

class Window {
  public:
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

// 2D vector for Euclid's geometry
class Vector {
  public:
    float x;
    float y;

    Vector() : x{0}, y{0} {}
    Vector(float x) : x{x}, y{0} {}
    Vector(float x, float y) : x{x}, y{y} {}
    Vector(const Vector &other) {
        if (&other != this) {
            x = other.x;
            y = other.y;
        }
    }
    Vector &operator=(const Vector &other) {
        if (&other != this) {
            x = other.x;
            y = other.y;
        }

        return *this;
    }

    ~Vector() {}

  private:
};

class Shape {
  public:
    Vector position;

    Shape() {}

    void draw() {}

    ~Shape() {}

  private:
};

class Point : Shape {
  public:
  private:
};

class Segment : Shape {
  public:
  private:
};

class Ray : Shape {
  public:
  private:
};

class Line : Shape {
  public:
  private:
};

class Angle : Shape {};

class Triangle : Shape {
  public:
  private:
};

class Renderer {
  public:
    Renderer() {}
    Renderer(const Renderer &other) = delete;
    Renderer &operator=(const Renderer &other) = delete;

    void draw_axis() {}
    void draw_shape() {}
    void draw() {}

    ~Renderer() {}

  private:
};

int main() {
    Window window(1280, 720, "Sandbox");
    GLint success;

    const GLchar *vertex_shader_src = ResourceManager::read_shader_src("./src/shaders/vertex.vert");

    if (!vertex_shader_src) {
        return -1;
    }

    const GLchar *fragment_shader_src =
        ResourceManager::read_shader_src("./src/shaders/fragment.frag");

    if (!fragment_shader_src) {
        return -1;
    }

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);

    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        char log[512];

        glGetShaderInfoLog(vertex_shader, 512, NULL, log);

        std::cout << "Vertex shader compilation error: " << log << std::endl;

        return -1;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE) {
        char log[512];

        glGetShaderInfoLog(fragment_shader, 512, NULL, log);

        std::cout << "Fragment shader compilation error: " << log << std::endl;

        return -1;
    }

    unsigned int program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        char log[512];

        glGetProgramInfoLog(program, 512, NULL, log);

        std::cout << "Program linking error: " << log << std::endl;

        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    unsigned int vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    float vertices[] = {0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

    unsigned int indices[] = {0, 1, 2, 3};

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window.get_window())) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);

        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

        // glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.get_window());
        glfwPollEvents();
    }

    delete vertex_shader_src;
    delete fragment_shader_src;

    return 0;
}
