#pragma once

#include <glm/glm.hpp>

struct AABB {
    glm::vec3 pos;
    glm::vec3 size;

    inline bool collidingWith(const AABB &other) {
        glm::vec3 min = pos;
        glm::vec3 max = pos + size;
        glm::vec3 otherMin = other.pos;
        glm::vec3 otherMax = other.pos + other.size;

        return
            min.x <= otherMax.x &&
            max.x >= otherMin.x &&
            min.y <= otherMax.y &&
            max.y >= otherMin.y &&
            min.z <= otherMax.z &&
            max.z >= otherMin.z;
    }
};