#version 330 core

in vec3 vTexCoord;

out vec4 color;

uniform samplerCube uCubemap;

void main() {
    color = texture(uCubemap, vTexCoord);
}