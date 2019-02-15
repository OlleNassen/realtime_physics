#include "texture.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

texture::texture(const int width,
    int height,
    wrap wrap_parameter,
    filter filter_parameter,
    format format_parameter,
    type type_parameter)
{
    generate(wrap_parameter, filter_parameter);

    glTexImage2D(GL_TEXTURE_2D, 0,
        static_cast<int>(format_parameter),
        width, height, 0,
        static_cast<unsigned int>(format_parameter),
        static_cast<unsigned int>(type_parameter), nullptr);
}

texture::texture(const std::string& path,
    wrap wrap_parameter,
    filter filter_parameter,
    format format_parameter,
    type type_parameter)
{
    generate(wrap_parameter, filter_parameter);

    auto width = 0;
    auto height = 0;
    auto nr_of_channels = 0;
    auto* data = stbi_load(path.c_str(),
        &width, &height, &nr_of_channels, 0);

	stbi_set_flip_vertically_on_load(true);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0,
        static_cast<int>(format_parameter),
        width, height, 0,
        static_cast<unsigned int>(format_parameter),
        static_cast<unsigned int>(type_parameter), data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
}

texture::texture(const std::vector<std::string>& paths,
    wrap wrap_parameter,
    filter filter_parameter,
    format format_parameter,
    type type_parameter)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    stbi_set_flip_vertically_on_load(false);

    auto width = 0;
    auto height = 0;
    auto nr_of_channels = 0;

    for (auto i = 0u; i < paths.size(); ++i)
    {
        auto* data = stbi_load(paths[i].c_str(),
            &width, &height, &nr_of_channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, static_cast<unsigned int>(format_parameter), width, height,
                0, static_cast<unsigned int>(format_parameter),
                static_cast<unsigned int>(type_parameter), data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: "
                << paths[i] << std::endl;

        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
        static_cast<int>(wrap_parameter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
        static_cast<int>(wrap_parameter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
        static_cast<int>(filter_parameter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
        static_cast<int>(filter_parameter));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
        static_cast<int>(filter_parameter));
}

texture::~texture()
{
    glDeleteTextures(1, &id);
}

void texture::uniform(const shader& shader, const std::string& name,
    int texture_index) const
{
    shader.uniform(name, texture_index);
    glActiveTexture(GL_TEXTURE0 + texture_index);
    glBindTexture(GL_TEXTURE_2D, id);
}

void texture::skybox(const shader& shader) const
{
    shader.uniform("skybox", 5);
    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void texture::bind_to_buffer() const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, id, 0);
}

void texture::generate(wrap wrap_parameter, filter filter_parameter)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        static_cast<int>(wrap_parameter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
        static_cast<int>(wrap_parameter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        static_cast<int>(filter_parameter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        static_cast<int>(filter_parameter));
}
