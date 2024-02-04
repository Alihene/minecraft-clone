#pragma once

#include "util/util.hpp"

#include <glm/glm.hpp>

struct ShaderProgram {
    u32 handle;

    ShaderProgram() = default;
    ShaderProgram(std::string vertexPath, std::string fragmentPath);

    void bind();

    void setInt(std::string location, i32 value);
    void setFloat(std::string location, f32 value);
    void setVec2(std::string location, glm::vec2 value);
    void setVec3(std::string location, glm::vec3 value);
    void setVec4(std::string location, glm::vec4 value);
    void setMat4(std::string location, glm::mat4 value);
};