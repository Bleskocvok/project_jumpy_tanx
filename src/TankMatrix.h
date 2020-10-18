
/*
 * File:   TankMatrix.h
 * Author: Frantisek Brablik
 *
 * Created on 17. dubna 2019, 22:14
 */

#pragma once

#ifndef TANKMATRIX_H
#define TANKMATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>

#include "ObjModel.h"
#include "JoinedPoints.h"

class TankMatrix {
public:
    TankMatrix(const Terrain* terrain
            , glm::vec3 centre
            , glm::vec3 size = glm::vec3(1.4f, 1.5f, 2.4f)
            , float half_head_size = 0.69f)
            : points(terrain)
            , half_head_size(half_head_size) {
        float width = size.x;//1.4;
        float length = size.z;//2.4;
        float height = size.y;//1.5;
        float small = 1.4;
        float big = 1.0;
        move_up_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -small * 0.5, 0.0f));

        points.add_point(centre + glm::vec3(-width * 0.5, 0.0, -length * 0.5), small);
        points.add_point(centre + glm::vec3(width * 0.5, 0.0, -length * 0.5), small);
        points.add_point(centre + glm::vec3(-width * 0.5, 0.0, length * 0.5), small);
        points.add_point(centre + glm::vec3(width * 0.5, 0.0, length * 0.5), small);
        points.add_point(centre + glm::vec3(0.0, height - small * 0.5, 0.0), big);
        points.join(0, 1);
        points.join(2, 3);
        points.join(0, 2);
        points.join(1, 3);
        points.join(0, 3);

        points.join(1, 2);

        points.join(0, 4);
        points.join(1, 4);
        points.join(2, 4);
        points.join(3, 4);
    }

    glm::mat4 get_matrix() {
        glm::vec3 middle = get_middle();
        return glm::inverse(glm::lookAt(middle, (points[0] + points[1]) * 0.5f, points[4] - middle)) * move_up_mat;
    }

    glm::mat4 get_head_matrix() {
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), cannon_angle, glm::vec3(0.0f, 1.0f, 0.0f));
        return get_matrix() * rotate * trans;
    }

    glm::mat4 get_cannon_matrix() {

        glm::mat4 rotate_up = glm::rotate(glm::mat4(1.0f), cannon_upward, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -half_head_size));
        return get_head_matrix() * trans * rotate_up;
    }

    void transform(glm::mat4 matrix) {
        points.transform(matrix);
    }

    void update(double time) {
        points.update(time);
    }

    glm::vec3 get_mass_center() {
        return (points[0] + points[1] + points[2] + points[3] + points[4]) * 0.2f;
    }

    glm::vec3 get_middle() {
        return (points[0] + points[1] + points[2] + points[3]) * 0.25f;
    }

    glm::vec3 get_front() {
        return (points[0] + points[1]) * 0.5f;
    }

    void set_cannon_rotation(float y_rotation) {
        glm::vec3 front_vec = get_front() - get_middle();
        glm::vec2 diff(front_vec.x, front_vec.z);

        float angle_diff = std::atan2(diff.x, diff.y);

        if ((points[4] - get_middle()).y < 0.0f) {
            y_rotation = -y_rotation;
            angle_diff = -angle_diff;
        }
        cannon_angle = -angle_diff + y_rotation;
        //cannon_upward = -x_rotation * 0.25f;
    }

    void set_cannon_upward(float x_rotation) {
        cannon_upward = x_rotation;
        if (cannon_upward < 0.0) {
            cannon_upward = 0.0;
        } else if (cannon_upward > 0.25f * M_PI) {
            cannon_upward = 0.25f * M_PI;
        }
    }

    void forward(float dist) {
//        glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::normalize(get_front() - get_middle()) * dist);
//        //glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.0f, 0.0f));
//        transform(trans);
        points.translate(glm::normalize(get_front() - get_middle()) * dist);
    }

    void turn(float angle) {
        glm::vec3 middle = get_middle();
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, points[4] - middle);
        points.translate(-middle);
        transform(rotate);
        points.translate(middle);
    }

    void push(glm::vec3 vec) {
        points.translate(vec);
    }

    bool on_ground() const {
        return points.on_ground();
    }

    glm::vec3 get_cannon_vector() {
        glm::vec3 result = glm::vec4(0.0f, 0.0f, 1.0f, -1.0f) * glm::inverse(get_cannon_matrix());
        return -result;
    }

    JoinedPoints points;
    glm::mat4 move_up_mat;
    float cannon_angle{};
    float cannon_upward{};
    float half_head_size;
};

#endif /* TANKMATRIX_H */

