#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// Bullet
#include <btBulletDynamicsCommon.h>

#define MS_PER_UPDATE_FRAME 16

const GLint WIDTH = 1280, HEIGHT = 720;

unsigned int VBO, VAO, EBO;

class Cube {
   public:
    float vertices_[36][3] = {
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},  {0.5f, 0.5f, -0.5f},   {0.5f, 0.5f, -0.5f},
        {-0.5f, 0.5f, -0.5f},  {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f},  {0.5f, -0.5f, 0.5f},
        {0.5f, 0.5f, 0.5f},    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f},   {-0.5f, -0.5f, 0.5f},
        {-0.5f, 0.5f, 0.5f},   {-0.5f, 0.5f, -0.5f},  {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, 0.5f},  {-0.5f, 0.5f, 0.5f},   {0.5f, 0.5f, 0.5f},    {0.5f, 0.5f, -0.5f},
        {0.5f, -0.5f, -0.5f},  {0.5f, -0.5f, -0.5f},  {0.5f, -0.5f, 0.5f},   {0.5f, 0.5f, 0.5f},
        {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},  {0.5f, -0.5f, 0.5f},   {0.5f, -0.5f, 0.5f},
        {-0.5f, -0.5f, 0.5f},  {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},  {0.5f, 0.5f, -0.5f},
        {0.5f, 0.5f, 0.5f},    {0.5f, 0.5f, 0.5f},    {-0.5f, 0.5f, 0.5f},   {-0.5f, 0.5f, -0.5f}};

    glm::vec3 scale_;        // Scale along each axis
    glm::quat orientation_;  // Rotation around each axis
    glm::vec3 position_;     // World position

    Cube(glm::vec3 scale, glm::quat orientation, glm::vec3 position)
        : scale_{scale}, orientation_{orientation}, position_{position} {}

    glm::mat4 ComputeModelMatrix() {
        glm::mat4 scale_matrix = glm::scale(glm::identity<glm::mat4>(), scale_);
        glm::mat4 rotation_matrix = glm::toMat4(orientation_);
        glm::mat4 translation_matrix = glm::translate(glm::identity<glm::mat4>(), position_);

        glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;

        return model_matrix;
    }
};

// class BulletDebugDrawer_OpenGL : public btIDebugDraw {
//    public:
//     void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix) {
//         glUniformMatrix4fv(glGetUniformLocation(lineShader.Program, "projection"), 1, GL_FALSE,
//                            glm::value_ptr(pProjectionMatrix));
//         glUniformMatrix4fv(glGetUniformLocation(lineShader.Program, "view"), 1, GL_FALSE,
//                            glm::value_ptr(pViewMatrix));
//     }

//     virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
//         // Vertex data
//         GLfloat points[12];

//         points[0] = from.x();
//         points[1] = from.y();
//         points[2] = from.z();
//         points[3] = color.x();
//         points[4] = color.y();
//         points[5] = color.z();

//         points[6] = to.x();
//         points[7] = to.y();
//         points[8] = to.z();
//         points[9] = color.x();
//         points[10] = color.y();
//         points[11] = color.z();

//         glDeleteBuffers(1, &VBO);
//         glDeleteVertexArrays(1, &VAO);
//         glGenBuffers(1, &VBO);
//         glGenVertexArrays(1, &VAO);
//         glBindVertexArray(VAO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
//                               (GLvoid *)(3 * sizeof(GLfloat)));
//         glBindVertexArray(0);

//         glBindVertexArray(VAO);
//         glDrawArrays(GL_LINES, 0, 2);
//         glBindVertexArray(0);
//     }
//     virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int,
//                                   const btVector3 &) {}
//     virtual void reportErrorWarning(const char *) {}
//     virtual void draw3dText(const btVector3 &, const char *) {}
//     virtual void setDebugMode(int p) { m = p; }
//     int getDebugMode(void) const { return 3; }
//     int m;
// };

class PhysicsWorld {
   public:
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void update(GLFWwindow *window, uint32_t dt, uint32_t tick) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void render(GLFWwindow *window, uint32_t program, uint32_t VAO, Cube &cube, uint32_t dt) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                       &cube.ComputeModelMatrix()[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

struct Shape {
    float vertices[4][3] = {
        {0.5f, 0.5f, 0.0f},    // top right
        {0.5f, -0.5f, 0.0f},   // bottom right
        {-0.5f, -0.5f, 0.0f},  // bottom left
        {-0.5f, 0.5f, 0.0f}    // top left
    };

    unsigned int indices[2][3] = {
        // note that we start from 0!
        {0, 1, 3},  // first triangle
        {1, 2, 3}   // second triangle
    };
};

unsigned int send_shape_to_gpu(Cube &cube) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube.vertices_), cube.vertices_, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shape.indices), shape.indices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);

    return VAO;
}

const char *vertex_shader_source = R""""(
#version 410

layout(location=0)in vec3 position;
layout(location=1)in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color_to_fragment;

void main(){
    gl_Position=projection*view*model*vec4(position,1.);

    color_to_fragment=vec4(color,1.);
}
)"""";

const char *fragment_shader_source = R""""(
#version 410

in vec4 color_to_fragment;

out vec4 color;

void main() {
    color = color_to_fragment;
}
)"""";

unsigned int create_vertex_shader() {
    unsigned int vertex_shader;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[1024];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 1024, NULL, infoLog);

        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return vertex_shader;
}

unsigned int create_fragment_shader() {
    unsigned int fragment_shader;

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    int success;
    char infoLog[1024];

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return fragment_shader;
}

unsigned int create_program(unsigned int vertex_shader, unsigned int fragment_shader) {
    unsigned int program;

    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    return program;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Could not initialize GLFW library" << std::endl;

        return -1;
    }

    glfwSetErrorCallback([](int code, const char *description) {
        std::cerr << "GLFW error: " << code << " " << description << std::endl;
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr, nullptr);

    if (!window) {
        std::cerr << "Could not create GLFW window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Could not initialize GLEW library" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vertex_shader = create_vertex_shader();
    unsigned int fragment_shader = create_fragment_shader();
    unsigned int program = create_program(vertex_shader, fragment_shader);

    Cube cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(0.0f, 0.0f, 0.0f, 1.0f),
              glm::vec3(0.0f, 0.0f, 0.0f));

    unsigned int VAO = send_shape_to_gpu(cube);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    uint32_t prev{0}, cur{0}, dt{0}, accum{0}, tick{0};

    prev = std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
               .count();

    while (!glfwWindowShouldClose(window)) {
        cur = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::system_clock::now().time_since_epoch())
                  .count();

        dt = cur - prev;
        prev = cur;
        accum += dt;

        while (accum >= MS_PER_UPDATE_FRAME) {
            update(window, dt, tick);

            accum -= MS_PER_UPDATE_FRAME;

            ++tick;
        }

        render(window, program, VAO, cube, accum / MS_PER_UPDATE_FRAME);
    }

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    glfwTerminate();

    return (0);
}

// btBroadphaseInterface *broadphase_;
//     btDefaultCollisionConfiguration *collision_configuration_;
//     btCollisionDispatcher *dispatcher_;
//     btSequentialImpulseConstraintSolver *solver_;
//     btDiscreteDynamicsWorld *world_;

//     broadphase_ = new btDbvtBroadphase();
//     collision_configuration_ = new btDefaultCollisionConfiguration();
//     dispatcher_ = new btCollisionDispatcher(collision_configuration_);
//     solver_ = new btSequentialImpulseConstraintSolver();
//     world_ =
//         new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collision_configuration_);

//     world_->setGravity(btVector3(0, -9.8, 0));

//     btCollisionShape *shape = new btCapsuleShape(0.5, 1);

//     btVector3 inertia = {0.0f, 0.0f, 0.0f};

//     shape->calculateLocalInertia(30.0f, inertia);

//     btMotionState *motion_state = new btDefaultMotionState();

//     btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(30.0f, motion_state,
//     shape,
//                                                                        inertia);

//     btRigidBody *body = new btRigidBody(rigidBodyConstructionInfo);

//     body->setFriction(0.5f);
//     body->setRestitution(0.0f);
//     body->setRollingFriction(0.5f);
//     body->setSpinningFriction(0.5f);

//     body->forceActivationState(DISABLE_DEACTIVATION);
//     body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

//     body->setUserIndex(0);

//     world_->addRigidBody(body);

// delete world_;
//     delete solver_;
//     delete dispatcher_;
//     delete collision_configuration_;
//     delete broadphase_;