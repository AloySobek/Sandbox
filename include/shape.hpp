#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Shape {
    glm::vec3 scale_;        // Scale along each axis
    glm::quat orientation_;  // Rotation around each axis
    glm::vec3 position_;     // World position

    Shape(glm::vec3 scale, glm::quat orientation, glm::vec3 position)
        : scale_{scale}, orientation_{orientation}, position_{position} {}

    void Scale(float scalar) { scale_ += scalar; }

    glm::mat4 ComputeModelMatrix() {
        glm::mat4 scale_matrix = glm::scale(glm::identity<glm::mat4>(), scale_);
        glm::mat4 rotation_matrix = glm::toMat4(orientation_);
        glm::mat4 translation_matrix = glm::translate(glm::identity<glm::mat4>(), position_);

        glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;

        return model_matrix;
    }
};

#endif