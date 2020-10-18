
/*
 * File:   Camera.h
 * Author: Frantisek Brablik
 *
 * Created on 66. dubna 2019, 6:66
 */

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(float distance)
		: distance(distance) {
		update();
	}

	void set_focus(glm::vec3 new_focus) {
		focus = new_focus;
		update();
	}
	void set_distance(float dist) {
		distance = dist;
		update();
	}
	void rotate(float y, float x) {
		rotation_x += x;
		rotation_y += y;
		if (rotation_x < -M_PI * 0.5 + 0.001f) {
			rotation_x = -M_PI * 0.5 + 0.001f;
		} else if (rotation_x > M_PI * 0.5 - 0.001f) {
			rotation_x = M_PI * 0.5 - 0.001f;
		}
		update();
	}
	const glm::mat4& view_matrix() const {
		return matrix;
	}
	const glm::vec3& eye_pos() const {
		return eye_position;
	}
	float get_rotation_y() const {
		return rotation_y;
	}
	float get_rotation_x() const {
		return rotation_x;
	}
	glm::vec3 xz_vector() const {
		glm::mat4 y_rot = glm::rotate(glm::mat4(1.0f), rotation_y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 looktor = glm::vec3(0.0f, 0.0f, distance);
		looktor = y_rot * glm::vec4(looktor, 1.0f);
		return looktor;
	}
private:
	glm::mat4 matrix{};
	glm::vec3 eye_position{};
	glm::vec3 focus{};
	float distance{};
	float rotation_x{};
	float rotation_y{ M_PI };

	void update() {
		glm::mat4 y_rot = glm::rotate(glm::mat4(1.0f), rotation_y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 x_rot = glm::rotate(glm::mat4(1.0f), rotation_x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 looktor = glm::vec3(0.0f, 0.0f, distance);
		looktor = y_rot * x_rot * glm::vec4(looktor, 1.0f);
		eye_position = focus - looktor;
		matrix = glm::lookAt(eye_position, eye_position + looktor, glm::vec3(0.0f, 1.0f, 0.0f));
	}
};

#endif /* CAMERA_H */

