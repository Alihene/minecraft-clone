#version 330 core

in vec2 aPos;
in vec2 aTexCoord;

out vec2 vTexCoord;

uniform mat4 uProj;

void main() {
    gl_Position = uProj * vec4(aPos, 0.0f, 1.0f);
    vTexCoord = aTexCoord;
}