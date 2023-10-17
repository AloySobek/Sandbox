#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera {
   public:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;

    Camera(glm::vec3 position)
        : position_{position},
          target_{glm::vec3(0.0f, 0.0f, -1.0f)},
          up_{glm::vec3(0.0f, 1.0f, 0.0f)} {}

    void Rotate(glm::quat rotation) { target_ = glm::normalize(target_ * rotation); }

    void MoveForward(float scalar) { position_ += scalar * target_; }
    void MoveBackward(float scalar) { position_ -= scalar * target_; }
    void MoveRight(float scalar) { position_ += glm::normalize(glm::cross(target_, up_)) * scalar; }
    void MoveLeft(float scalar) { position_ -= glm::normalize(glm::cross(target_, up_)) * scalar; }

    glm::mat4 GetViewMatrix() { return glm::lookAt(position_, position_ + target_, up_); }

    ~Camera() {}
};

#endif