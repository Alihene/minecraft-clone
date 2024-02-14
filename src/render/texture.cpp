#include "texture.hpp"

#include "gl.hpp"

#include <stb_image/stb_image.h>

Texture::Texture(std::string path, TextureFormat format) : format(format) {
    glGenTextures(1, &handle);
    bind();

    stbi_set_flip_vertically_on_load(true);

    i32 width, height, channels;
    u8 *data;

    if(format == FORMAT_RGB) {
        data = stbi_load(path.c_str(), &width, &height, &channels, 3);
    } else if(format == FORMAT_RGBA) {
        data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    }

    if(!data) {
        std::cerr << "Failed to load image " << path << std::endl;
        std::exit(-1);
    }

    if(format == FORMAT_RGB) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if(format == FORMAT_RGBA) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
}

void Texture::bind() {
    bind(GL_TEXTURE_2D);
}

void Texture::bind(u32 target) {
    glBindTexture(target, handle);
}

void Texture::activate(u8 index) {
    glActiveTexture(GL_TEXTURE0 + index);
}