
/*
 * File:   ModelRef.h
 * Author: Frantisek Brablik
 *
 * Created on 21. dubna 2019, 0:12
 */

#pragma once

#ifndef MODELREF_H
#define MODELREF_H

#include "ObjModel.h"
#include "Material.h"
#include "Program.h"
#include "Texture.h"

#include "ShaderLoc.h"

class ModelRef {
public:
    ObjModel* model = nullptr;
	Material* material = nullptr;
	Texture* dif_tex = nullptr;
	Texture* light_tex = nullptr;

    ModelRef() = default;

	ModelRef(ObjModel* model, Material* mat, Texture* dif_tex, Texture* light_tex)
			: model(model)
			, material(mat)
			, dif_tex(dif_tex)
            , light_tex(light_tex) {}

	void draw(Program& program) {
		if (material) {
			material->send(program);
		}
		if (dif_tex) {
			dif_tex->bindUnit(Locs::DIF_TEXTURE);
		}

        if (light_tex) {
            light_tex->bindUnit(Locs::LIGHT_TEXTURE);
            glProgramUniform1i(program.index(), Locs::USE_LIGHT_TEX, true);
        } else {
            glProgramUniform1i(program.index(), Locs::USE_LIGHT_TEX, false);
        }

		glProgramUniformMatrix4fv(program.index(), Locs::MODEL, 1, false, glm::value_ptr(model_matrix));

        if (model) {
            model->draw();
        }
	}
	void set_matrix(glm::mat4 matrix) {
		model_matrix = matrix;
	}
private:
	glm::mat4 model_matrix{ 1.0f };
};

#endif /* MODELREF_H */

