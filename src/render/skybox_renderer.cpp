#include "skybox_renderer.hpp"

#include "gl.hpp"
#include "state.hpp"

#include <vector>

#include <stb_image/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

SkyboxRenderer::SkyboxRenderer(Camera *camera) : camera(camera)
{
    program = ShaderProgram("shaders/skybox.vert", "shaders/skybox.frag");

    glGenTextures(1, &cubemap.handle);
    cubemap.bind(GL_TEXTURE_CUBE_MAP);

    std::vector<std::string> cubemapNames = {
        "textures/middlesb.png",
        "textures/middlesb.png",
        "textures/topsb.png",
        "textures/bottomsb.png",
        "textures/middlesb.png",
        "textures/middlesb.png"};

    stbi_set_flip_vertically_on_load(false);

    i32 i = 0;

    for (std::string fileName : cubemapNames)
    {
        i32 width, height, channels;
        u8 *data;

        data = stbi_load(std::string(state.resourceDir + fileName).c_str(), &width, &height, &channels, 4);

        if (!data) {
            std::cerr << "Failed to load cubemap texture " << fileName << std::endl;
            std::exit(-1);
        }

        GLuint target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;

        glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        i++;

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*) 0);
    glEnableVertexAttribArray(0);
}

void SkyboxRenderer::renderSkybox()
{
    f32 skyboxVertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glDepthMask(GL_FALSE);

    program.bind();
    program.setMat4("uModel", glm::scale(glm::mat4(1.0f), glm::vec3(500.0f)));
    program.setMat4("uView", glm::mat4(glm::mat3(camera->view)));
    program.setMat4("uProj", camera->proj);
    program.setInt("uCubemap", 0);

    cubemap.bind(GL_TEXTURE_CUBE_MAP);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
}