
/*
 * File:   Enemy.h
 * Author: Frantisek Brablik
 *
 * Created on 5. května 2019, 22:10
 */

#pragma once

#ifndef ENEMY_H
#define ENEMY_H

#include <cmath>

#include "Tank.h"
#include "Projectiles.h"
#include "RandGen.h"

#include "Debug.h"

class Enemy {
public:
    Tank tank;

    Enemy(const Terrain* terrain,
            glm::vec3 centre,
            glm::vec3 size,
            ModelRef torso,
            ModelRef head,
            ModelRef cannon,
            Projectiles& projectiles)
            : tank(terrain, centre, size, torso, head, cannon)
            , projectiles(projectiles)
            , terrain(terrain) {

        material = tank.torso.material;
        tank.torso.material = &material;
        tank.head.material = nullptr;
        tank.cannon.material = nullptr;
    }

    virtual ~Enemy() = default;

    void aim(glm::vec3 pos) {
        player_pos = pos;
    }

    virtual void update(double time) {
        tank.update(time);
    }

    void draw(Program& program, double time) {
        tank.draw(program);
    }

protected:
    Projectiles& projectiles;
    const Terrain* terrain;
    glm::vec3 player_pos;
    glm::vec3 move_pos;
    RandGen rand_gen;

    Material material;
};

class NormalEnemy : public Enemy {
public:
    NormalEnemy(const Terrain* terrain,
            glm::vec3 centre,
            glm::vec3 size,
            ModelRef torso,
            ModelRef head,
            ModelRef cannon,
            Projectiles& projectiles)
            : Enemy(terrain, centre, size, torso, head, cannon, projectiles) {

        next_location();
    }

    void update(double time) override {
        Enemy::update(time);

        /*
         * Movement shenanigans.
         */
        countdown -= time;
        if (countdown < 0.0 || glm::distance(move_pos, tank.get_mass_center()) <= OK_DIST) {
            next_location();
        }

        glm::vec3 tank_vec = tank.get_tank_vector();
        float tank_angle = std::atan2(tank_vec.x, tank_vec.z);

        glm::vec3 way_vec = move_pos - tank.get_mass_center();
        float way_angle = std::atan2(way_vec.x, way_vec.z);

        if (std::fabs(tank_angle - way_angle) < OK_ANGLE) {
            if (!okay_angle) {
                spin_left = !spin_left;
            }
            okay_angle = true;
        } else {
            okay_angle = false;
            tank.turn(spin_left ? TURN_SPEED : -TURN_SPEED);
        }

        tank.forward(SPEED);

        /*
         * Cannon rotation.
         */
        glm::vec3 to_player_vec = player_pos - tank.get_mass_center();
        float to_player_angle = std::atan2(to_player_vec.x, to_player_vec.z);

        tank.set_cannon_rotation(to_player_angle);
        float dist_to_player = glm::distance(player_pos, tank.get_mass_center());
        float x_cannon_angle = (dist_to_player / UPWARD_ANGLE_MAX_DISTANCE);
        tank.set_cannon_upward(x_cannon_angle * M_PI * 0.25f);


        /*
         * Shooting.
         */
        shoot_countdown -= time;
        float color_coeff = 1.0 - (shoot_countdown / SHOOT_TIME);
        color_coeff *= color_coeff * color_coeff * color_coeff * color_coeff;
        glm::vec3 red(1.0f, 0.2f, 0.0f);
        glm::vec3 normal(1.0f, 1.0f, 1.0f);
        glm::vec3 result = normal * (1.0f - color_coeff) + color_coeff * red;
        material.set_diffuse(result.x, result.y, result.z);
        material.set_specular(result.x, result.y, result.z);
        if (shoot_countdown <= 0.0) {
//            float power = 1.0;
//            float gravity = JoinedPoints::GRAVITY * 0.8;
//            float x_diff = glm::distance(glm::vec2(player_pos), glm::vec2(tank.get_mass_center()));
//            float y_diff = tank.get_mass_center().y - player_pos.y;
//            float d = std::pow(power, 4.0f) - gravity * (gravity * x_diff * x_diff + 2.0f * y_diff * power * power);
//            d = -d;
//            if (d >= 0) { // Can't sqrt(x) negative x.
//                float angle1 = std::atan((power * power + sqrt(d)) / (gravity * x_diff));
                //float angle2 = std::atan((power * power - sqrt(d)) / (gravity * x_diff));
          float val1 = x_cannon_angle * M_PI * 0.25f * 0.4f;
            float val2 = x_cannon_angle * M_PI * 0.25f * 1.0f;
          float chosen_angle = rand_gen.get_float(std::min(val1, val2), std::max(val1, val2));
                glm::vec2 plane = glm::normalize(glm::vec2(to_player_vec.x, to_player_vec.z));
                glm::vec3 shoot_vec(plane.x * std::cos(chosen_angle) * SHOOT_POWER, SHOOT_POWER * std::sin(chosen_angle), plane.y * std::cos(chosen_angle) * SHOOT_POWER);
                projectiles.add(tank.get_mass_center(), shoot_vec);
//            }
            shoot_countdown += SHOOT_TIME;
        }

    }

private:
    static constexpr float SPEED = 0.006f;
    static constexpr float TURN_SPEED = 0.004f;
    static constexpr float COUNTDOWN_TIME = 20.0;
    static constexpr float SHOOT_TIME = 5.0;
    static constexpr float SHOOT_POWER = 1.0;

    static constexpr float OK_DIST = 5.0f;
    static constexpr float OK_ANGLE = 0.1f;

    static constexpr float UPWARD_ANGLE_MAX_DISTANCE = 40.0;

    bool spin_left = true;
    bool okay_angle = false;

    double countdown = COUNTDOWN_TIME;
    double shoot_countdown = SHOOT_TIME;

    void next_location() {
        float x = rand_gen.get_float(terrain->x_min(), terrain->x_max());
        float z = rand_gen.get_float(terrain->z_min(), terrain->z_max());
        move_pos = glm::vec3(x, terrain->get_height(x, z), z);
        countdown = COUNTDOWN_TIME;
    }
};

#endif /* ENEMY_H */

