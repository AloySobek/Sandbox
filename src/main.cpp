#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>

#include "window.hpp"

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

int main() {
    Window window(1280, 720, "Sandbox");
    GLint success;

    const GLchar *vertex_shader_src = read_shader_src("./src/shaders/vertex.vert");

    if (!vertex_shader_src) {
        return -1;
    }

    const GLchar *fragment_shader_src = read_shader_src("./src/shaders/fragment.frag");

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
