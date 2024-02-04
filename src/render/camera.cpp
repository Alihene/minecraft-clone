#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "state.hpp"

void Camera::update() {
    if(state.window->cursorActive) {
        return;
    }

    f32 x = state.window->mouse.x;
    f32 y = state.window->mouse.y;

    if(!state.window->mouse.hasMoved) {
        state.window->mouse.lastX = x;
        state.window->mouse.lastY = y;
        state.window->mouse.hasMoved = true;
    }

    f32 xOffset = x - state.window->mouse.lastX;
    f32 yOffset = state.window->mouse.lastY - y;
    state.window->mouse.lastX = x;
    state.window->mouse.lastY = y;

    f32 sens = 0.1f;
    xOffset *= sens;
    yOffset *= sens;

    yaw += xOffset;
    pitch += yOffset;

    if(pitch > 89.0f) {
	    pitch = 89.0f;
    } else if (pitch < -89.0f) {
	    pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(direction);

    this->view = glm::lookAt(this->pos, this->pos + this->front, this->up);
    this->proj = glm::perspective(glm::radians(80.0f), state.window->aspectRatio(), 0.1f, 1000.0f);
}