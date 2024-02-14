#pragma once

#include "util/util.hpp"

struct Texture {
    u32 handle;

    enum TextureFormat {
        FORMAT_RGB,
        FORMAT_RGBA
    };

    TextureFormat format;

    Texture() = default;
    Texture(std::string path, TextureFormat format);

    void bind();
    void bind(u32 target);

    static void activate(u8 index);
};