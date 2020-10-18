
/*
 * File:   Lights.h
 * Author: Frantisek Brablik
 *
 * Created on 2. května 2019, 20:41
 */

#pragma once

#ifndef LIGHTS_H
#define LIGHTS_H

#include "utils/graphics.h"

#include <vector>
#include <memory>

#include "Program.h"

class Lights {
public:
    struct OneLight {
        glm::vec4 pos; // .w == 0.0 -> directional light; .w == 1.0 -> point light
        glm::vec4 amb_col;
        glm::vec4 dif_col;
        glm::vec4 spec_col; // .w - radius
        bool operator==(const OneLight& right) const {
            return pos == right.pos && amb_col == right.amb_col && dif_col == right.dif_col && spec_col == right.spec_col;
        }
    };

    class LightRef {
    public:
        OneLight* light = nullptr;

        LightRef() = default;

        void erase() {
            if (keep) {
                *keep = false;
            }
        }
    private:
        friend Lights;

        int* keep = nullptr;

        LightRef(OneLight* light, int* keep)
                : light(light)
                , keep(keep) {}
    };

    Lights() {
        add({
                glm::vec4(0.0f, 3.0f, 2.0f, 0.0f),
                glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        });

//        transformed.clear();
//        std::vector<OneLight> transformed;
//        for (auto& light_ptr : lights_vec) {
//            transformed.push_back(*light_ptr);
//        }
//
//        glCreateBuffers(1, &buffer);
//        glNamedBufferStorage(buffer, transformed.size() * sizeof(OneLight), transformed.data(), GL_DYNAMIC_STORAGE_BIT);
    }
    ~Lights() {
        glDeleteBuffers(1, &buffer);
    }
    void update() {
        for (size_t i = 0; i < lights_vec.size(); ++i) {
            if (!(*keep_lights[i])) {
                lights_vec.erase(lights_vec.begin() + i);
                keep_lights.erase(keep_lights.begin() + i);
                --i;
            }
        }

        if (lights_vec.empty()) {
            add({
                { 0.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f, 0.0f } });
        }

        transformed.clear();
        std::vector<OneLight> transformed;
        for (auto& light_ptr : lights_vec) {
            transformed.push_back(*light_ptr);
        }

        glDeleteBuffers(1, &buffer);
        glCreateBuffers(1, &buffer);
        glNamedBufferStorage(buffer, transformed.size() * sizeof(OneLight), transformed.data(), GL_DYNAMIC_STORAGE_BIT);
    }
    void send(Program& program) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer);
    }
    LightRef add(OneLight new_light) {
        lights_vec.emplace_back(new OneLight(new_light));
        keep_lights.emplace_back(new int(true));
        assert(new_light == *lights_vec.back());
        return LightRef(lights_vec.back().get(), keep_lights.back().get());
    }
    LightRef add() {
        return add({ glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                glm::vec4(1.0f, 1.0f, 1.0f, 16.0f)
        });
    }
private:
    std::vector<std::unique_ptr<OneLight>> lights_vec;
    std::vector<std::unique_ptr<int>> keep_lights;
    GLuint buffer = 0;
    std::vector<OneLight> transformed;
};

#endif /* LIGHTS_H */

