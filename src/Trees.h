
/*
 * File:   Trees.h
 * Author: Frantisek Brablik
 *
 * Created on 20. května 2019, 21:42
 */

#pragma once

#ifndef TREES_H
#define TREES_H

#include "Terrain.h"
#include "Lights.h"
#include "ModelRef.h"
#include "RandGen.h"

#include <vector>

class Trees {
public:
    Trees(Terrain& terrain, Lights& lights, unsigned seed, int count = 10)
            : terrain(terrain)
            , lights(lights) {
       
        bush_mat.set_ambient(0.1, 0.1, 0.3)
                .set_diffuse(1.0, 1.0, 1.0)
                .set_specular(0.2, 0.2, 1.0)
                .set_shine(100.0);
       
        RandGen rand{ seed };
        for (int i = 0; i < count; ++i) {
            float x = rand.get_float(terrain.x_min(), terrain.x_max());
            float z = rand.get_float(terrain.z_min(), terrain.z_max());
            float y = terrain.get_height(x, z);
            bushes.emplace_back(glm::vec3{ x, y, z}, rand.get_float(0.0, M_PI * 2.0));
//            lights.add({
//                { x, y + 10.0, z, 1.0 },
//                { 0.0, 0.0, 0.3, 1.0 },
//                { 0.0, 0.3, 113.0, 1.0 },
//                { 0.1, 0.3, 130.0, 20.0 } });
            lights.add({
                { x, y + 10.0, z, 1.0 },
                { 0.0, 0.0, 1.0, 1.0 },
                { 0.9, 0.9, 20.0, 1.0 },
                { 0.3, 0.3, 1.0, 20.0 } });
        }
       
        for (int i = 0; i < count; ++i) {
            float x = rand.get_float(terrain.x_min(), terrain.x_max());
            float z = rand.get_float(terrain.z_min(), terrain.z_max());
            float y = terrain.get_height(x, z);
            mushrooms.emplace_back(glm::vec3{ x, y, z}, rand.get_float(0.0, M_PI * 2.0));
        }
    }
   
    void draw(Program& program) {
        for (size_t i = 0; i < bushes.size(); ++i) {
            light_bush_ref.set_matrix(glm::translate(glm::mat4(1.0f), bushes[i].first)
                    * glm::rotate(glm::mat4(1.0f), bushes[i].second, { 0.0f, 1.0f, 0.0f }));
            light_bush_ref.draw(program);
        }
        for (size_t i = 0; i < mushrooms.size(); ++i) {
            mushroom_ref.set_matrix(glm::translate(glm::mat4(1.0f), mushrooms[i].first)
                    * glm::rotate(glm::mat4(1.0f), mushrooms[i].second, { 0.0f, 1.0f, 0.0f }));
            mushroom_ref.draw(program);
        }
    }
   
private:
    Terrain& terrain;
    Lights& lights;
   
    Material bush_mat = Material();
    Texture bush_texture{ "models/light_bush_texture.png" };
    Texture bush_light_map{ "models/light_bush_light_map.png" };
    ObjModel light_bush{ "models/light_bush.obj" };
    ModelRef light_bush_ref{ &light_bush, &bush_mat, &bush_texture, &bush_light_map };
   
    std::vector<std::pair<glm::vec3, float>> bushes;
   
    Material mushroom_mat = Material();
    Texture mushroom_texture{ "models/tree_texture.png" };
    Texture mushroom_light_map{ "models/tree_light_map.png" };
    ObjModel mushroom{ "models/tree.obj" };
    ModelRef mushroom_ref{ &mushroom, &mushroom_mat, &mushroom_texture, &mushroom_light_map };
   
    std::vector<std::pair<glm::vec3, float>> mushrooms;
};

#endif /* TREES_H */

