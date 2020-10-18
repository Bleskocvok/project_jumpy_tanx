
/*
 * File:   JoinedPoints.h
 * Author: Frantisek Brablik
 *
 * Created on 18. dubna 2019, 22:32
 */

#pragma once

#ifndef JOINEDPOINTS_H
#define JOINEDPOINTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Terrain.h"

class JoinedPoints {
public:
    static constexpr float GRAVITY = 1.8f * 48.0f;
   
	JoinedPoints(const Terrain* terrain, float ground_drag = 0.07, float air_drag = 0.04)
			: terrain(terrain)
            , ground_drag(ground_drag)
            , air_drag(air_drag) {
        points.reserve(16);
    }
	
	void update(double time) {
        ground = false;
		for (size_t r = 0; r < 5; ++r) {
            for (size_t i = 0; i < points.size(); ++i) {
				calc_collision(i);
			}
			for (auto& one : links) {
				solve_link(one);
			}
		}
        for (size_t i = 0; i < points.size(); ++i) {
			inertia(i, time);
		}
	}
   
	void transform(glm::mat4 matrix) {
		for (size_t i = 0; i < points.size(); ++i) {
			glm::vec4 result = glm::vec4(points[i], 1.0) * matrix;
			points[i] = glm::vec3(result.x, result.y, result.z);
		}
	}
   
	void translate(glm::vec3 vec) {
		for (size_t i = 0; i < points.size(); ++i) {
			points[i] += vec;
		}
	}
   
	void add_point(glm::vec3 point, float size) {
		points.push_back(point);
		prev_points.push_back(point);
		sizes.push_back(size * 0.5);
	}
   
	void join(size_t one, size_t two) {
		float distance = glm::length(points[one] - points[two]);
		links.push_back(link(points[one], points[two], distance));
	}
   
	glm::vec3& operator[](size_t index) {
		return points[index];
	}
   
    const glm::vec3& operator[](size_t index) const {
		return points[index];
	}
   
	size_t size() const {
		return points.size();
	}
   
    float size(size_t index) const {
        return sizes[index];
    }
   
    bool on_ground() const {
        return ground;
    }
   
    void push(JoinedPoints& other) {
        for (size_t i = 0; i < points.size(); ++i) {
            for (size_t k = 0; k < other.size(); ++k) {
                glm::vec3 diff = other[k] - points[i];
                float distance = glm::length(diff);
                diff = glm::normalize(diff);
                float length = sizes[i] + other.size(k);
                if (length > distance) {
                    float travel = (distance - length) * 0.5;
                    points[i] += travel * diff;
                    other[k] -= travel * diff;
                }
            }
        }
    }
   
    unsigned collides(glm::vec3 pos, float radius) {
        unsigned count = 0;
        for (size_t i = 0; i < size(); ++i) {
            count += static_cast<unsigned>(glm::distance(pos, points[i]) < radius + sizes[i]);
        }
        return count;
    }
private:
	struct link {
		glm::vec3& one;
		glm::vec3& two;
		float length;
		link(glm::vec3& one, glm::vec3& two, float length)
				: one(one)
				, two(two)
				, length(length) {}
	};
	
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> prev_points;
	std::vector<float> sizes;
	std::vector<link> links;
	const Terrain* terrain;
    bool ground = false;
    float ground_drag = 0.07;
    float air_drag = 0.04;
	
	void inertia(size_t index, double time) {
		glm::vec3 diff = points[index] - prev_points[index];
		prev_points[index] = points[index];
        if (ground) {
//            points[index] += diff * 0.93f;
            points[index] += diff * (1.0f - ground_drag);
        } else {
//            points[index] += diff * 0.96f;
            points[index] += diff * (1.0f - air_drag);
        }
		points[index].y -= time * time * GRAVITY;
	}
   
	void calc_collision(size_t index) {
		glm::vec3& point = points[index];
		float radius = sizes[index];
		float height = terrain->get_height(point.x, point.z);
        if (point.y - radius < height) {
			point.y = height + radius;
            ground = true;
		}
        if (point.x - radius < terrain->x_min()) {
            point.x = terrain->x_min() + radius;
        }
        if (point.x + radius > terrain->x_max()) {
            point.x = terrain->x_max() - radius;
        }
        if (point.z - radius < terrain->z_min()) {
            point.z = terrain->z_min() + radius;
        }
        if (point.z + radius > terrain->z_max()) {
            point.z = terrain->z_max() - radius;
        }
	}
   
	void solve_link(link& chain) {
		glm::vec3 diff = chain.two - chain.one;
		float distance = glm::length(diff);
		diff = glm::normalize(diff);
		float travel = (distance - chain.length) * 0.5;
		chain.one += travel * diff;
		chain.two -= travel * diff;
	}
};

#endif /* JOINEDPOINTS_H */

