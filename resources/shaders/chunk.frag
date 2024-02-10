#version 330 core

in vec2 vTexCoord;
in float vBrightness;

out vec4 color;

uniform sampler2D uTexture;

void main() {
    color = texture(uTexture, vTexCoord) * vec4(vBrightness, vBrightness, vBrightness, 1.0);
}