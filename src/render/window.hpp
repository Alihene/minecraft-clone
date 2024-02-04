#pragma once

#include "gl.hpp"
#include "util/util.hpp"

#include <glm/glm.hpp>

struct Window {
    GLFWwindow *handle;

    glm::uvec2 dimensions;

    bool cursorActive = false;

    struct {
        f32 x, y, lastX, lastY = 0.0f;
        bool hasMoved = false;
    } mouse;

    Window() = default;
    Window(std::string name, u32 width, u32 height);

    void destroy();

    void endFrame();

    bool shouldClose();

    bool keyPressed(i32 key);

    f32 aspectRatio();
};