#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform {
   public:
    glm::vec3 scale_;        // Scale along each axis
    glm::quat orientation_;  // Rotation around each axis
    glm::vec3 position_;     // World position

    Transform(glm::vec3 scale, glm::quat orientation, glm::vec3 position)
        : scale_{scale}, orientation_{orientation}, position_{position} {}

    Transform(btTransform &transform) {
        scale_ = glm::vec3(1.0f, 1.0f, 1.0f);

        FromBtTransform(transform);
    }

    void Scale(glm::vec3 scale_vector) {
        scale_.x *= scale_vector.x;
        scale_.y *= scale_vector.y;
        scale_.z *= scale_vector.z;
    }

    void Rotate(glm::quat rotation) { orientation_ *= rotation; }

    void Translate(glm::vec3 translate_vector) { position_ += translate_vector; }

    glm::mat4 GetModelMatrix() {
        glm::mat4 scale_matrix = glm::scale(glm::identity<glm::mat4>(), scale_);
        glm::mat4 rotation_matrix = glm::toMat4(orientation_);
        glm::mat4 translation_matrix = glm::translate(glm::identity<glm::mat4>(), position_);

        glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix;

        return model_matrix;
    }

    void FromBtTransform(btTransform transform) {
        orientation_.x = transform.getRotation().getX();
        orientation_.y = transform.getRotation().getY();
        orientation_.z = transform.getRotation().getZ();
        orientation_.w = transform.getRotation().getW();

        position_.x = transform.getOrigin().getX();
        position_.y = transform.getOrigin().getY();
        position_.z = transform.getOrigin().getZ();
    }
};

#endif