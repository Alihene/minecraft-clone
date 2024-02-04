#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "util/util.hpp"

struct Camera {
    glm::vec3 pos;

    glm::vec3 up;
    glm::vec3 front;

    glm::mat4 view;
    glm::mat4 proj;

    f32 pitch = 0.0f;

    f32 yaw = -90.0f;

    Camera() :
        pos(glm::vec3(0.0f)),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)),
        view(glm::mat4(1.0f)),
        proj(glm::mat4(1.0f)) {}

    void update();
};