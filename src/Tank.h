
/*
 * File:   Tank.h
 * Author: Frantisek Brablik
 *
 * Created on 21. dubna 2019, 22:58
 */

#pragma once

#ifndef TANK_H
#define TANK_H

#include "TankMatrix.h"
#include "ModelRef.h"
#include "Debug.h"

class Enemy;
class NormalEnemy;

class Tank {
public:
	Tank(const Terrain* terrain,
			glm::vec3 centre,
			glm::vec3 size,
			ModelRef torso,
			ModelRef head,
			ModelRef cannon)
	: tank_matrix(terrain, centre, size)
	, torso(torso)
	, head(head)
	, cannon(cannon) {
        set_cannon_upward(cannon_angle);
    }
	
	void update(double time) {
		tank_matrix.update(time);
		torso.set_matrix(tank_matrix.get_matrix());
		head.set_matrix(tank_matrix.get_head_matrix());
		cannon.set_matrix(tank_matrix.get_cannon_matrix());
	}
   
	void draw(Program& program) {
		torso.draw(program);
		head.draw(program);
		cannon.draw(program);
	}
   
	void forward(float dist) {
		tank_matrix.forward(dist);
	}
   
    void jump() {
        if (tank_matrix.on_ground()) {
            tank_matrix.push(glm::vec3(0.0f, 1.0f, 0.0));
        }
    }
   
	void turn(float angle) {
		tank_matrix.turn(angle);
	}
   
	void set_cannon_rotation(float y_rotation) {
		tank_matrix.set_cannon_rotation(y_rotation);
	}
   
    void set_cannon_upward(float x_roattion) {
        tank_matrix.set_cannon_upward(x_roattion);
    }
   
    void move_cannon(float diff) {
        cannon_angle += diff;
        cannon_angle = std::min(std::max(cannon_angle, 0.0f), 0.2f * static_cast<float>(M_PI));
        tank_matrix.set_cannon_upward(cannon_angle);
    }
   
	glm::vec3 get_mass_center() {
		return tank_matrix.get_mass_center();
	}
   
	glm::vec3 get_tank_vector() {
		return tank_matrix.get_front() - tank_matrix.get_middle();
	}
   
    glm::vec3 get_cannon_vector() {
        return tank_matrix.get_cannon_vector();
    }
   
    bool dead() const {
        return health <= 0;
    }
   
    void hit(int dmg) {
        health -= dmg;
    }
   
    float collides(glm::vec3 pos, float radius) {
        return tank_matrix.points.collides(pos, radius) / static_cast<float>(tank_matrix.points.size());
    }
   
    bool squash(Tank& other) {
        if (!other.tank_matrix.on_ground() && collides(other)) {
            hit(health);
            return true;
        }
        return false;
    }
   
    void push(Tank& other) {
        tank_matrix.points.push(other.tank_matrix.points);
    }
   
private:
    friend Enemy;
    friend NormalEnemy;
   
	TankMatrix tank_matrix;
	ModelRef torso;
	ModelRef head;
	ModelRef cannon;
    int health = 100;
    float cannon_angle = 0.2 * M_PI;
   
    bool collides(Tank& other) {
        for (size_t i = 0; i < other.tank_matrix.points.size(); ++i) {
            if (tank_matrix.points.collides(other.tank_matrix.points[i], other.tank_matrix.points.size(i))) {
                return true;
            }
        }
        return false;
    }
};

#endif /* TANK_H */

