#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#define GLEW_STATIC
#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#ifndef CMAKE_SOURCE_DIR
#define CMAKE_SOURCE_DIR ""
#endif

class Program {
   public:
    GLuint program_;

    Program() {
        const char *vertex_shader_source_buffer =
            ReadShaderSource((std::string(CMAKE_SOURCE_DIR) + "/shaders/vertex.vert").c_str());
        const char *fragment_shader_source_buffer =
            ReadShaderSource((std::string(CMAKE_SOURCE_DIR) + "/shaders/fragment.frag").c_str());

        CreateShaders(vertex_shader_source_buffer, fragment_shader_source_buffer);
        CreateProgram();
    }

    void SetModelMatrix(glm::mat4 model_matrix) {
        glUniformMatrix4fv(glGetUniformLocation(program_, "model"), 1, GL_FALSE,
                           &model_matrix[0][0]);
    }

    void SetViewMatrix(glm::mat4 view_matrix) {
        glUniformMatrix4fv(glGetUniformLocation(program_, "view"), 1, GL_FALSE, &view_matrix[0][0]);
    }

    void SetProjectionMatrix(glm::mat4 projection_matrix) {
        glUniformMatrix4fv(glGetUniformLocation(program_, "projection"), 1, GL_FALSE,
                           &projection_matrix[0][0]);
    }

    void Use() { glUseProgram(program_); }

    ~Program() {
        glDeleteProgram(program_);
        glDeleteShader(fragment_shader_);
        glDeleteShader(vertex_shader_);
    }

   private:
    GLuint vertex_shader_;
    GLuint fragment_shader_;

    void CreateShaders(const char *vertex_shader_source_buffer,
                       const char *fragment_shader_source_buffer) {
        GLint success;
        char log_buffer[512];

        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertex_shader_, 1, &vertex_shader_source_buffer, NULL);
        glShaderSource(fragment_shader_, 1, &fragment_shader_source_buffer, NULL);

        glCompileShader(vertex_shader_);
        glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(vertex_shader_, 512, NULL, log_buffer);

            throw std::runtime_error(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n") +
                                     log_buffer);
        }

        glCompileShader(fragment_shader_);
        glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(fragment_shader_, 512, NULL, log_buffer);

            throw std::runtime_error(std::string("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n") +
                                     log_buffer);
        }
    }

    void CreateProgram() {
        GLint success;
        char log_buffer[512];

        program_ = glCreateProgram();

        glAttachShader(program_, vertex_shader_);
        glAttachShader(program_, fragment_shader_);

        glLinkProgram(program_);

        glGetProgramiv(program_, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(program_, 512, NULL, log_buffer);

            throw std::runtime_error(std::string("ERROR::PROGRAM::LINKING_FAILED\n") + log_buffer);
        }
    }

    const char *ReadShaderSource(const char *shader_path) {
        std::ifstream file;

        file.open(shader_path, std::ios::in | std::ios::ate);

        if (!file.is_open()) {
            throw std::runtime_error(std::string("Could not open file ") + shader_path);
        }

        int length = file.tellg();
        file.seekg(std::ios::beg);
        char *buffer = new char[length];
        file.read(buffer, length);

        return const_cast<const char *>(buffer);
    }
};

#endif