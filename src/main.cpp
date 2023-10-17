#include <btBulletDynamicsCommon.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "camera.hpp"
#include "physics.hpp"
#include "program.hpp"
#include "transform.hpp"
#include "window.hpp"

#define MS_PER_UPDATE_FRAME 16

int main() {
  std::unique_ptr<Window> window;
  std::unique_ptr<Program> program;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<Physics> physics;

  try {
    window.reset(new Window(1280, 720, "OpenGL"));

    program.reset(new Program());

    camera.reset(new Camera(glm::vec3(0.0f, 0.0f, 5.0f)));

    physics.reset(new Physics());
  } catch (std::exception &error) {
    spdlog::error(error.what());

    return (-1);
  }

  Vertices capsule_vertices = physics->GetCapsuleVertices();
  spdlog::info("Capsule: {}", capsule_vertices.size);
  Vertices box_vertices = physics->GetBoxVertices();
  spdlog::info("Box: {}", box_vertices.size);

  GLfloat *vertices =
      new GLfloat[capsule_vertices.size * 3 + box_vertices.size * 3];
  memcpy(vertices, capsule_vertices.vertices,
         sizeof(GLfloat) * 3 * capsule_vertices.size);
  memcpy(vertices + capsule_vertices.size * 3, box_vertices.vertices,
         sizeof(GLfloat) * 3 * box_vertices.size);

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(GLfloat) *
                   (capsule_vertices.size * 3 + box_vertices.size * 3),
               vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  btTransform capsule_transform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
                                btVector3(0.0f, 0.0f, 0.0f));
  btTransform box_transform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f),
                            btVector3(10.0f, 0.0f, 0.0f));

  physics->SpawnCapsule(0, capsule_transform);
  physics->SpawnBox(0, box_transform);

  Transform transform1(capsule_transform);
  Transform transform2(box_transform);

  // program->Use();
  program->SetProjectionMatrix(glm::perspective(
      glm::radians(45.0f), (float)window->width_ / (float)window->height_, 0.1f,
      100.0f));

  uint32_t prev{0}, cur{0}, dt{0}, accum{0}, tick{0};

  prev = std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count();

  while (!glfwWindowShouldClose(window->window_)) {
    cur = std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();

    dt = cur - prev;
    prev = cur;
    accum += dt;

    while (accum >= MS_PER_UPDATE_FRAME) {
      if (glfwGetKey(window->window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window->window_, true);
      }

      if (glfwGetKey(window->window_, GLFW_KEY_W) == GLFW_PRESS) {
        camera->MoveForward(0.1f);
      }

      if (glfwGetKey(window->window_, GLFW_KEY_S) == GLFW_PRESS) {
        camera->MoveBackward(0.1f);
      }

      if (glfwGetKey(window->window_, GLFW_KEY_D) == GLFW_PRESS) {
        camera->MoveRight(0.1f);
      }

      if (glfwGetKey(window->window_, GLFW_KEY_A) == GLFW_PRESS) {
        camera->MoveLeft(0.1f);
      }

      physics->Tick();

      accum -= MS_PER_UPDATE_FRAME;

      ++tick;
    }

    static double last_x = 0, last_y = 0;

    double x, y;

    glfwGetCursorPos(window->window_, &x, &y);

    if (last_x == 0)
      last_x = x;
    if (last_y == 0)
      last_y = y;

    if (glfwGetMouseButton(window->window_, GLFW_MOUSE_BUTTON_1) ==
        GLFW_PRESS) {
      camera->Rotate(glm::angleAxis(glm::radians(float(y - last_y) * 0.1f),
                                    glm::vec3(1.0f, 0.0f, 0.0f)));

      camera->Rotate(glm::angleAxis(glm::radians(float(x - last_x) * 0.1f),
                                    glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    last_y = y;
    last_x = x;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program->program_);

    glBindVertexArray(VAO);

    program->SetViewMatrix(camera->GetViewMatrix());

    transform1.Rotate(
        glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    program->SetModelMatrix(transform1.GetModelMatrix());

    glDrawArrays(GL_LINES, 0, capsule_vertices.size);

    transform2.Rotate(
        glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    transform2.Translate(glm::vec3(-0.01f, 0.0f, 0.0f));

    program->SetModelMatrix(transform2.GetModelMatrix());

    glDrawArrays(GL_LINES, capsule_vertices.size,
                 capsule_vertices.size + box_vertices.size);

    glfwSwapBuffers(window->window_);
    glfwPollEvents();
  }

  return (0);
}

// unsigned int EBO;
// glGenBuffers(1, &EBO);
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape.indices), shape.indices,
// GL_STATIC_DRAW); glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

// glBindVertexArray(0);
