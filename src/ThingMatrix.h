
/*
 * File:   ThingMatrix.h
 * Author: Frantisek Brablik
 *
 * Created on 2. května 2019, 12:42
 */

#pragma once

#ifndef THINGMATRIX_H
#define THINGMATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "JoinedPoints.h"

class ThingMatrix {
public:
    ThingMatrix(const Terrain* terrain, glm::vec3 centre, glm::vec3 size, float radiuses) // radii
            : points(terrain) {

        points.add_point(centre + glm::vec3(0.0f, 0.0f, -size.z * 0.5f), radiuses);
        points.add_point(centre + glm::vec3(-size.x * 0.5f, 0.0f, +size.z * 0.5f), radiuses);
        points.add_point(centre + glm::vec3(size.x * 0.5f, 0.0f, +size.z * 0.5f), radiuses);
        points.add_point(centre + glm::vec3(0.0f, size.y, 0.0f), radiuses);

        points.join(0, 1);
        points.join(1, 2);
        points.join(2, 0);

        points.join(0, 3);
        points.join(1, 3);
        points.join(2, 3);
    }

    void update(double time) {
        points.update(time);
    }

    glm::mat4 get_matrix() {
        glm::vec3 middle = get_middle();
        return glm::inverse(glm::lookAt(middle, (points[0] + points[1]) * 0.5f, points[3] - middle));
    }

    void explode(glm::vec3 centre, float power) {
        for (size_t i = 0; i < points.size(); ++i) {
            glm::vec3 diff = points[i] - centre;
            float dist2 = glm::length(diff);
            //float dist2 = diff.x * diff.x + diff.y * diff.y
            if (dist2 <= 5.0 * power) {
                points[i] += diff / dist2 * power * 0.5f;
            }
        }
    }
private:
    JoinedPoints points;

    glm::vec3 get_middle() const {
        return (points[0] + points[1] + points[2]) * 0.3333f;
    }
};

#endif /* THINGMATRIX_H */

