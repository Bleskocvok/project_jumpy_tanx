
/*
 * File:   Material.h
 * Author: Frantisek Brablik
 *
 * Created on 13. dubna 2019, 20:12
 */

#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "Program.h"
#include "ShaderLoc.h"

class Material {
public:
    Material(bool send_shine = true)
            : send_shine(send_shine) {}
	Material(GLint ambient_loc, GLint diffuse_loc, GLint specular_loc)
		: ambient_loc(ambient_loc), diffuse_loc(diffuse_loc), specular_loc(specular_loc) {
		send_shine = false;
	}
	Material(GLint ambient_loc, GLint diffuse_loc, GLint specular_loc, GLint shine_loc)
		: ambient_loc(ambient_loc), diffuse_loc(diffuse_loc), specular_loc(specular_loc), shine_loc(shine_loc) {
		send_shine = true;
	}
	Material& set_ambient(float r, float g, float b) {
		ambient_r = r;
		ambient_g = g;
		ambient_b = b;
		return *this;
	}
	Material& set_diffuse(float r, float g, float b) {
		diffuse_r = r;
		diffuse_g = g;
		diffuse_b = b;
		return *this;
	}
	Material& set_specular(float r, float g, float b) {
		specular_r = r;
		specular_g = g;
		specular_b = b;
		return *this;
	}
	Material& set_ambient_loc(GLint loc) {
		ambient_loc = loc;
		return *this;
	}
	Material& set_diffuse_loc(GLint loc) {
		diffuse_loc = loc;
		return *this;
	}
	Material& set_specular_loc(GLint loc) {
		specular_loc = loc;
		return *this;
	}
	Material& set_shine_loc(GLint loc) {
		shine_loc = loc;
		send_shine = true;
		return *this;
	}
	Material& set_shine(float val) {
		shine = val;
		send_shine = true;
		return *this;
	}
	Material& unset_shine() {
		send_shine = false;
		return *this;
	}
	void send(const Program& program) const {
		GLuint index = program.index();
		glProgramUniform3f(index, ambient_loc, ambient_r, ambient_g, ambient_b);
		glProgramUniform3f(index, diffuse_loc, diffuse_r, diffuse_g, diffuse_b);
		glProgramUniform3f(index, specular_loc, specular_r, specular_g, specular_b);
		if (send_shine) {
			glProgramUniform1f(index, shine_loc, shine);
        }
	}
private:
	GLint ambient_loc = Locs::MAT_AMB,
            diffuse_loc = Locs::MAT_DIF,
            specular_loc = Locs::MAT_SPEC,
            shine_loc = Locs::MAT_SHINE;
	float ambient_r = 0.2, ambient_g = 0.2, ambient_b = 0.2;
	float diffuse_r = 1.0, diffuse_g = 1.0, diffuse_b = 1.0;
	float specular_r = 1.0, specular_g = 1.0, specular_b = 1.0;
	float shine = 20.0;
	bool send_shine = false;
};

#endif /* MATERIAL_H */

