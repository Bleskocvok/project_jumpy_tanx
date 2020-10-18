
/*
 * File:   Texture.h
 * Author: Frantisek Brablik
 *
 * Created on 13. dubna 2019, 0:19
 */

#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "utils/graphics.h"

#include <stb_image.h>

#include <cmath>
#include <string>

class Texture {
public:
    Texture(const std::string& file)
        : texture(load(file)) {}

    ~Texture() { glDeleteTextures(1, &texture); }

    void bindUnit(GLuint binding) { glBindTextureUnit(binding, texture); }

private:
    GLuint texture;

    GLuint load(const std::string& filename) {
        int width = 0;
        int height = 0;
        int channels = 0;

        // use stb_image to load the image data
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 4);

        GLuint result;
        glCreateTextures(GL_TEXTURE_2D, 1, &result);

        // assign the image data to the texture
        glTextureStorage2D(result,
                std::log2(width),
                GL_RGBA8,
                width,
                height);
        glTextureSubImage2D(result,
                0,                  // level of detail
                0, 0,                  // x, y offsets
                width, height,
                GL_RGBA,              // format
                GL_UNSIGNED_BYTE,      // data type of pixel data
                data);

        stbi_image_free(data);

        // generate mipmaps
        glGenerateTextureMipmap(result);

        // set parameters
        glTextureParameteri(result, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(result, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return result;
    }
};

#endif /* TEXTURE_H */

