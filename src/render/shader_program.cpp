#include "shader_program.hpp"
#include "state.hpp"

#include <vector>

ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath) {
    u32 vertexHandle = glCreateShader(GL_VERTEX_SHADER);
    u32 fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexSource = readFile(state.resourceDir + vertexPath);
    std::string fragmentSource = readFile(state.resourceDir + fragmentPath);

    const char *vertexSourcePtr = vertexSource.c_str();
    const char *fragmentSourcePtr = fragmentSource.c_str();

    i32 result;
    i32 logLength;

    glShaderSource(vertexHandle, 1, &vertexSourcePtr, nullptr);
    glCompileShader(vertexHandle);

    glGetShaderiv(vertexHandle, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexHandle, GL_INFO_LOG_LENGTH, &logLength);

    if(result == GL_FALSE) {
        std::vector<char> error(logLength + 1);
        glGetShaderInfoLog(vertexHandle, logLength, nullptr, error.data());
        std::cerr << "Failed to compile vertex shader: " << error.data() << std::endl;
        std::exit(-1);
    }

    glShaderSource(fragmentHandle, 1, &fragmentSourcePtr, nullptr);
    glCompileShader(fragmentHandle);

    glGetShaderiv(fragmentHandle, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentHandle, GL_INFO_LOG_LENGTH, &logLength);

    if(result == GL_FALSE) {
        std::vector<char> error(logLength + 1);
        glGetShaderInfoLog(fragmentHandle, logLength, nullptr, error.data());
        std::cerr << "Failed to compile fragment shader: " << error.data() << std::endl;
        std::exit(-1);
    }

    handle = glCreateProgram();
    glAttachShader(handle, vertexHandle);
    glAttachShader(handle, fragmentHandle);
    glLinkProgram(handle);

    glGetProgramiv(handle, GL_LINK_STATUS, &result);
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);

    if(logLength > 0) {
        std::vector<char> error(logLength + 1);
        glGetProgramInfoLog(handle, logLength, nullptr, error.data());
        std::cerr << "Failed to link program: " << error.data() << std::endl;
        std::exit(-1);
    }

    glDetachShader(handle, vertexHandle);
    glDetachShader(handle, fragmentHandle);

    glDeleteShader(vertexHandle);
    glDeleteShader(fragmentHandle);
}

void ShaderProgram::bind() {
    glUseProgram(handle);
}

void ShaderProgram::setInt(std::string location, i32 value) {
    glUniform1i(glGetUniformLocation(handle, location.c_str()), value);
}

void ShaderProgram::setFloat(std::string location, f32 value) {
    glUniform1f(glGetUniformLocation(handle, location.c_str()), value);
}

void ShaderProgram::setVec2(std::string location, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(handle, location.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(std::string location, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(handle, location.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(std::string location, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(handle, location.c_str()), 1, &value[0]);
}

void ShaderProgram::setMat4(std::string location, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(handle, location.c_str()), 1, GL_FALSE, &value[0][0]);
}