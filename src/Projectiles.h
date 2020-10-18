
/*
 * File:   Projectiles.h
 * Author: Frantisek Brablik
 *
 * Created on 28. dubna 2019, 16:59
 */

#pragma once

#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "ModelRef.h"
#include "Terrain.h"
#include "Material.h"
#include "ShaderLoc.h"
#include "Particles.h"
#include "Lights.h"
#include "Explosions.h"

class Projectiles {
public:
    Projectiles(Terrain& terrain, Particles& particles, Explosions& explosions, Lights& lights, float size)
            : terrain(terrain)
            , particles(particles)
            , explosions(explosions)
            , lights(lights)
            , radius(size * 0.5) {

        material.set_ambient(0.2, 0.2, 0.2)
                .set_diffuse(1.0, 1.0, 1.0)
                .set_specular(0.0, 0.0, 0.0)
                .set_shine(1.0);
    }
    void add(glm::vec3 pos, glm::vec3 vec) {
        projs.emplace_back(pos, pos - vec);
        light_vec.push_back(lights.add({
                { 0.0f, 0.0f, 0.0f, 1.0f },
                { 0.2f, 0.2f, 0.2f, 0.2f },
                { 0.4f, 0.2f, 0.4f, 1.0f },
                { 1.0f, 1.0f, 1.0f, 16.0f } }));
    }
    void update(double time) {
        for (int i = projs.size() - 1; i >= 0; i--) {
            if (destroyed(projs[i].first, radius)) {
                glm::vec3 pos = projs[i].first;
                projs.erase(projs.begin() + i);

                particles.add(pos, 1.0f);

                explosions.add(pos);

                light_vec[i].erase();
                light_vec.erase(light_vec.begin() + i);
            } else {
                glm::vec3 current = projs[i].first;
                glm::vec3 prev = projs[i].second;
                glm::vec3 diff = current - prev;
                prev = current;
                current.y -= 1.8 * time * time * 48.0;
                projs[i] = { current + diff, prev };

                light_vec[i].light->pos = glm::vec4(current, light_vec[i].light->pos.w);
            }
        }
    }
    void draw(Program& program, double time) {
        for (size_t i = 0; i < projs.size(); ++i) {
            glm::vec3 result = projs[i].first * (float)(1.0 - time) + projs[i].second * (float)time;
            glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), result);
            model.set_matrix(model_mat);
            model.draw(program);
        }
    }
private:
    Terrain& terrain;
    Particles& particles;
    Explosions& explosions;
    Lights& lights;
    ObjModel proj_model{ "models/ball.obj" };
    Texture proj_texture{ "models/ball_texture.png" };
    Texture proj_light{ "models/ball_light_map.png" };
    Material material{ Locs::MAT_AMB, Locs::MAT_DIF, Locs::MAT_SPEC, Locs::MAT_SHINE };
    ModelRef model{ &proj_model, &material, &proj_texture, &proj_light };
    std::vector<std::pair<glm::vec3, glm::vec3>> projs;
    std::vector<Lights::LightRef> light_vec;
    float radius;

    bool destroyed(glm::vec3 pos, float radius) {
        if (pos.y - radius < terrain.get_height(pos.x, pos.z)) {
            return true;
        }
        return pos.x < terrain.x_min()
                || pos.x > terrain.x_max()
                || pos.z < terrain.z_min()
                || pos.z > terrain.z_max();
    }
};

#endif /* PROJECTILES_H */

