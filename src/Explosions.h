
/*
 * File:   Explosions.h
 * Author: Frantisek Brablik
 *
 * Created on 8. května 2019, 18:35
 */

#pragma once

#ifndef EXPLOSIONS_H
#define EXPLOSIONS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Tank.h"

class Explosions {
public:
    Explosions(float radius, int dmg)
            : radius(radius)
            , dmg(dmg) {}

    void clear() {
        spheres.clear();
    }

    void add(glm::vec3 pos) {
        spheres.push_back(pos);
    }

    void calc_tank(Tank& tank) {
        for (auto& one : spheres) {
            float how_much = tank.collides(one, radius);
            tank.hit(static_cast<int>(how_much * dmg));
        }
    }
private:
    std::vector<glm::vec3> spheres;
    float radius;
    int dmg;
};

#endif /* EXPLOSIONS_H */

