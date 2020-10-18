
/*
 * File:   TerrainModel.h
 * Author: Frantisek Brablik
 *
 * Created on 19. dubna 2019, 22:33
 */

#pragma once

#ifndef TERRAINMODEL_H
#define TERRAINMODEL_H

#include <glm/glm.hpp>
#include <iostream>

#include "utils/mesh.hpp"

#include "Terrain.h"
#include "ModelRef.h"

class TerrainModel {
public:
	TerrainModel(Terrain& terrain, float step, ModelRef mats)
			: model(load(terrain, step))
            , mats(mats)
//            , wall(load_wall(terrain))
            {}
	
	void draw(Program& program) {
        mats.draw(program);
		model.draw();
//        tex_wall.bindUnit(0);
//        wall.draw();
	}
private:
	Mesh model;
    ModelRef mats;
   
//    Texture tex_wall{ "models/wall.png" };
//    Mesh wall;
   
    Mesh load_wall(Terrain& terrain) {
        std::vector<float> points;
		std::vector<float> normals;
		std::vector<float> coords;
		std::vector<uint32_t> indices;
       
        float height = terrain.x_max() - terrain.x_min();
        float ydiff = -height * 0.1;
        points.push_back(terrain.x_min());
        points.push_back(height + ydiff);
        points.push_back(terrain.z_min());
       
        points.push_back(terrain.x_max());
        points.push_back(height + ydiff);
        points.push_back(terrain.z_min());
       
        points.push_back(terrain.x_max());
        points.push_back(0.0 + ydiff);
        points.push_back(terrain.z_min());
       
        points.push_back(terrain.x_min());
        points.push_back(0.0 + ydiff);
        points.push_back(terrain.z_min());
       
       
        normals.push_back(0.0);
        normals.push_back(0.0);
        normals.push_back(1.0);
       
        normals.push_back(0.0);
        normals.push_back(0.0);
        normals.push_back(1.0);
       
        normals.push_back(0.0);
        normals.push_back(0.0);
        normals.push_back(1.0);
       
        normals.push_back(0.0);
        normals.push_back(0.0);
        normals.push_back(1.0);
       
       
        coords.push_back(0.0);
        coords.push_back(0.0);
       
        coords.push_back(1.0);
        coords.push_back(0.0);
       
        coords.push_back(1.0);
        coords.push_back(1.0);
       
        coords.push_back(0.0);
        coords.push_back(1.0);
       
       
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(1);
       
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(1);
       
        return Mesh(points, normals, coords, indices);
    }
	
	Mesh load(Terrain& terrain, float step) {
		std::vector<float> points;
		std::vector<float> normals;
		std::vector<float> coords;
		std::vector<uint32_t> indices;
		
		float tiny_bit = step * 0.25;
		
		size_t result_size = 0;
		for (float z = 0; z <= terrain.size(); z += step) {
			++result_size;
			for (float x = 0; x <= terrain.size(); x += step) {
				glm::vec3 vec_one(tiny_bit * 2.0, terrain.get_height(x + tiny_bit, z) - terrain.get_height(x - tiny_bit, z), 0.0f);
				glm::vec3 vec_two(0.0f, terrain.get_height(x, z + tiny_bit) - terrain.get_height(x, z - tiny_bit), tiny_bit * 2.0);
				
//				glm::vec3 vec_one(step, terrain.get_height(x + step, z) - terrain.get_height(x, z), 0.0f);
//				glm::vec3 vec_two(0.0f, terrain.get_height(x, z + step) - terrain.get_height(x, z), step);
				
				points.push_back(x);
				points.push_back(terrain.get_height(x, z));
				points.push_back(z);
				
				glm::vec3 normal = -glm::normalize(glm::cross(vec_one, vec_two));
				normals.push_back(normal.x);
				normals.push_back(normal.y);
				normals.push_back(normal.z);
				
				coords.push_back(x / (float)terrain.size());
				coords.push_back(z / (float)terrain.size());
			}
		}
		
		unsigned repeat = result_size - 1;
		for (size_t y = 0; y < repeat; ++y) {
			for (size_t x = 0; x < repeat; ++x) {
				indices.push_back(y * result_size + x);
				indices.push_back((y + 1) * result_size + x);
				indices.push_back(y * result_size + x + 1);
				
				indices.push_back((y + 1) * result_size + x);
				indices.push_back((y + 1) * result_size + x + 1);
				indices.push_back(y * result_size + x + 1);
			}
		}
		return Mesh(points, normals, coords, indices);
	}
};

#endif /* TERRAINMODEL_H */

