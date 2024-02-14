#include "window.hpp"

#include "state.hpp"

static void glfwErrorCallback(i32 err, const char *msg) {
    std::cerr << "GLFW Error (" << err << "): " << msg << std::endl;
    std::exit(-1);
}

static void glfwFramebufferSizeCallback(GLFWwindow *w, i32 width, i32 height) {
    Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(w));
    window->dimensions = glm::uvec2(width, height);
    glViewport(0, 0, width, height);
}

static void glfwCursorPosCallback(GLFWwindow *w, f64 x, f64 y) {
    Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(w));
    window->mouse.x = static_cast<f32>(x);
    window->mouse.y = static_cast<f32>(y);
}

static void glfwKeyCallback(GLFWwindow* w, i32 key, i32 scancode, i32 action, i32 mods) {
    Window *window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(w));
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if(window->cursorActive) {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        window->cursorActive = !window->cursorActive;
    }
    
    if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        Renderer *renderer = state.renderer;
        if(renderer->wireframeRender) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        renderer->wireframeRender = !renderer->wireframeRender;
    }
    
    //Keys 1-9
    if(key - 48 > 0 && key < 58) {
        state.player->hotbar.activeSlot = key - 48;
    }
}

static void glfwMouseButtonCallback(GLFWwindow *w, i32 button, i32 action, i32 mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if(!state.world) {
            return;
        }

        state.player->tryBreakBlock();
    } else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if(!state.world) {
            return;
        }

        state.player->tryPlaceBlock();
    }
}

Window::Window(std::string name, u32 width, u32 height) : dimensions(width, height) {
    glfwSetErrorCallback(glfwErrorCallback);

    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    handle = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

    if(!handle) {
        std::cerr << "Failed to create window" << std::endl;
        std::exit(-1);
    }

    glfwMakeContextCurrent(handle);

    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowUserPointer(handle, this);
    glfwSetFramebufferSizeCallback(handle, glfwFramebufferSizeCallback);
    glfwSetCursorPosCallback(handle, glfwCursorPosCallback);
    glfwSetKeyCallback(handle, glfwKeyCallback);
    glfwSetMouseButtonCallback(handle, glfwMouseButtonCallback);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to load OpenGL" << std::endl;
        std::exit(-1);
    }

    glfwShowWindow(handle);
}

void Window::destroy() {
    glfwTerminate();
}

void Window::endFrame() {
    glfwSwapBuffers(handle);
    glfwPollEvents();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(handle);
}

bool Window::keyPressed(i32 key) {
    return glfwGetKey(handle, key) == GLFW_PRESS;
}

f32 Window::aspectRatio() {
    return (f32) dimensions.x / (f32) dimensions.y;
}