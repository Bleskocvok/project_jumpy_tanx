
/*
 * File:   TextRenderer.h
 * Author: Frantisek Brablik
 *
 * Created on 13. května 2019, 15:03
 */

#pragma once

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "utils/graphics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <utility>

#include "utils/mesh.hpp"

#include "Texture.h"
#include "Program.h"
#include "Debug.h"

class TextRenderer {
public:
    TextRenderer(const std::string& atlas_file
            , int grid_size
            , std::string chars
            , float distance = 2.0)
            //
            : atlas(atlas_file)
            , grid_size(grid_size)
            , count(chars.size())
            , chars(std::move(chars))
            , ratio(720 / static_cast<float>(1280))
            , char_dist(distance) {

        float size = 1.0 / static_cast<float>(grid_size);
        for (int i = 0; i < count; ++i) {
            int x = i % grid_size;
            int y = i / grid_size;
            float x_coord = x * size;
            float y_coord = y * size;
            models.emplace_back(
                    std::vector<float>{
                        -1.0, 1.0, 0.0,     // 0
                        1.0, 1.0, 0.0,      // 1
                        -1.0, -1.0, 0.0,    // 2
                        1.0, -1.0, 0.0      // 3
                    },
                    std::vector<float>{
                        0.0, 0.0, 1.0,
                        0.0, 0.0, 1.0,
                        0.0, 0.0, 1.0,
                        0.0, 0.0, 1.0
                    },
                    std::vector<float>{
                        x_coord, y_coord,
                        x_coord + size, y_coord,
                        x_coord, y_coord + size,
                        x_coord + size, y_coord + size
                    },
                    std::vector<uint32_t>{
                        0, 3, 1,
                        2, 3, 0
                    });
        }
    }

    void draw_text(const std::string& text, float x, float y, float height) {
        text_shader.use();
        atlas.bindUnit(0);
        float width = height * ratio;
//        float text_size = text.size();
        float start_x = x;// - (width * text_size) * 0.5 + width * 0.5;
        for (size_t i = 0; i < text.size(); ++i) {
            auto found = chars.find(text[i]);
            if (found != std::string::npos) {
                glm::mat4 model_matrix{ 1.0 };
                model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(start_x + i * width * char_dist, y, 0.0f))
                        * glm::scale(glm::mat4(1.0f), glm::vec3(width, height, height));
                glProgramUniformMatrix4fv(
                        text_shader.index(),
                        Locs::MODEL,
                        1,
                        false,
                        glm::value_ptr(model_matrix));
                models[found].draw();
            }
        }
    }

    void resize(int width, int height) {
        ratio = height / static_cast<float>(width);
    }

private:
    std::vector<Mesh> models;
    Texture atlas;
    int grid_size, count;
    std::string chars;
    float ratio;
    float char_dist;
    Program text_shader{ "shaders/text.vert", "shaders/text.frag" };
};

#endif /* TEXTRENDERER_H */

