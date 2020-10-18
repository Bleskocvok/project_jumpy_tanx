
/*
 * File:   Particles.h
 * Author: Frantisek Brablik
 *
 * Created on 28. dubna 2019, 22:26
 */

#pragma once

#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include <utility>

#include "ModelRef.h"
#include "Terrain.h"
#include "ThingMatrix.h"
#include "RandGen.h"

class Particles {
public:
    Particles(Terrain* terrain, float explosion_size = 4)
            : terrain(terrain)
            , explosion_size(explosion_size) {

        explosion_mat.set_diffuse(1.25, 1.25, 1.25);
    }
    void update(double time) {
        for (int i = particles.size() - 1; i >= 0; --i) {
            particles[i].update(time);
        }
        for (int i = explosions.size() - 1; i >= 0; --i) {
            if (explosions[i].first >= EXPLOSION_TIME) {
                explosions.erase(explosions.begin() + i);
            } else {
                explosions[i].first += time;
            }
        }
    }
    void draw(Program& program, double time) {
        for (size_t i = 0; i < particles.size(); ++i) {
            models[indexes[i]].set_matrix(particles[i].get_matrix());
            models[indexes[i]].draw(program);
        }
        for (auto& expl : explosions) {
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), expl.second);
            float size = (std::sin(expl.first / EXPLOSION_TIME * M_PI * 0.5)) * explosion_size * 0.7 + explosion_size * 0.3;
//            float size = expl.first / EXPLOSION_TIME * explosion_size * 0.5 + explosion_size * 0.5;
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size, size, size));
            explosion_ref.set_matrix(trans * scale);
            explosion_ref.draw(program);
        }
    }
    void add(glm::vec3 pos, float power) {
        unsigned count = power * 5;
        for (unsigned i = 0; i < count; ++i) {
            glm::vec3 current_pos{
                    rand.get_float(-power, power),
                    rand.get_float(0.0f, power * 2.0),
                    rand.get_float(-power, power) };
            add_particle(0, pos, current_pos, power);
        }
        add_explosion(pos);
    }

    void tank_explosion(glm::vec3 pos, float power = 1.0) {
        unsigned count = power * 15;
        for (unsigned i = 0; i < count; ++i) {
            glm::vec3 current_pos{
                    rand.get_float(-power, power),
                    rand.get_float(0.0f, power * 2.0),
                    rand.get_float(-power, power) };
            add_particle(1, pos, current_pos, power * rand.get_float(0.95 * power, 1.64 * power));
        }
        unsigned count_expl = power * 7;
        for (unsigned i = 0; i < count_expl; ++i) {
            glm::vec3 current_pos{
                    rand.get_float(-power * 1.4, power * 1.4),
                    rand.get_float(0.0f, power * 2.8),
                    rand.get_float(-power * 1.4, power * 1.4) };
            add_explosion(pos + current_pos);
        }
    }

private:
    static constexpr unsigned MAX_PARTICLES = 150;
    static constexpr float EXPLOSION_TIME = 0.4;

    Terrain* terrain;

    float explosion_size;

    Texture particle_1_tex{ "models/dirt_1_texture.png" };
    Texture particle_1_light{ "models/neutral_light_map.png" };
    Material mat{ true };
    ObjModel particle_1_model{ "models/dirt_1.obj" };
    ModelRef particle_1{ &particle_1_model, &mat, &particle_1_tex, &particle_1_light };

    Texture tank_particle_1_tex{ "models/tank_particle_texture.png" };
    ObjModel tank_particle_1_model{ "models/tank_particle.obj" };

    std::vector<ModelRef> models{
        particle_1,
        ModelRef(&tank_particle_1_model, &mat, &tank_particle_1_tex, nullptr)
    };

    std::vector<ThingMatrix> particles;
    std::vector<size_t> indexes;

    ObjModel explosion_model{ "models/explosion.obj" };
    Texture explosion_tex{ "models/explosion_texture.png" };
    Texture explosion_light_map{ "models/green.png" };
    Material explosion_mat{ true };
    ModelRef explosion_ref{ &explosion_model, &explosion_mat, &explosion_tex, &explosion_light_map };

    std::vector<std::pair<float, glm::vec3>> explosions;

    RandGen rand;

    void add_explosion(glm::vec3 pos) {
        explosions.emplace_back(0.0, pos);
    }

    void add_particle(size_t type, glm::vec3 centre, glm::vec3 offset, float power) {
        if (particles.size() >= MAX_PARTICLES) {
            size_t index = rand.get_int(0, particles.size()-1);
            ThingMatrix& current = particles[index];
            current = ThingMatrix(terrain, centre + offset, glm::vec3(1.0f, 0.5f, 1.0f), 0.1f);
            current.explode(centre, power);
            indexes[index] = type;
        } else {
            particles.emplace_back(terrain, centre + offset, glm::vec3(1.0f, 0.5f, 1.0f), 0.1f);
            particles.back().explode(centre, power);
            indexes.push_back(type);
        }
    }
};

#endif /* PARTICLES_H */

