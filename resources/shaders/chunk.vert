#version 330 core

in vec3 aPos;
in vec2 aTexCoord;
in float aBrightness;

out vec2 vTexCoord;
out float vBrightness;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
    vTexCoord = aTexCoord;
    vBrightness = aBrightness;
}