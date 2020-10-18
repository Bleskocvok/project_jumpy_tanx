
/*
 * File:   Postprocess.h
 * Author: Frantisek Brablik
 *
 * Created on 9. kvetna 2019, 14:20
 */

#pragma once

#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "Program.h"
#include "FBO.h"

class Postprocess {
public:
    Postprocess(int width = 1280, int height = 720)
            : main(width, height)
            , bright_parts(width, height)
            , blurred1(width, height)
            , blurred2(width, height) {}

    void bind() {
        main.bind();
        main.clear();
    }

    //  void unbind() {}
    void clearColor(float r, float g, float b, float a) {
        main.clearColor(r, g, b, a);
    }

    void draw() {
        bright_parts.bind();
        bright_parts.clear();
        bright.use();
        glBindTextureUnit(0, main.color_texture());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        blurred1.bind();
        blurred1.clear();
        blur_h.use();
        glBindTextureUnit(0, bright_parts.color_texture());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        blurred2.bind();
        blurred2.clear();
        blur_v.use();
        glBindTextureUnit(0, blurred1.color_texture());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        add.use();
        glBindTextureUnit(0, main.color_texture());
        glBindTextureUnit(1, blurred2.color_texture());
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void resize(int width, int height) { main.resize(width, height);
        bright_parts.resize(width, height);
        blurred1.resize(width, height);
        blurred2.resize(width, height);
    }

private:
    FBO main;
    FBO bright_parts;
    FBO blurred1;
    FBO blurred2;

    Program bright{ "shaders/post_main.vert", "shaders/bright.frag" };
    Program blur_h{ "shaders/post_main.vert", "shaders/blur_horizontal.frag" };
    Program blur_v{ "shaders/post_main.vert", "shaders/blur_vertical.frag" };
    Program add{ "shaders/post_main.vert", "shaders/add.frag" };
};

#endif /* POSTPROCESS_H */
