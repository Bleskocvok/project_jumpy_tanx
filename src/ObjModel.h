
/*
 * File:   ObjModel.h
 * Author: Frantisek Brablik
 *
 * Created on 13. dubna 2019, 19:59
 */

#pragma once

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "utils/mesh.hpp"

#include <string>

class ObjModel {
public:
    ObjModel(const std::string& file)
        : meshes(Mesh::load_file(file)) {}

    void draw() {
        for (auto& piece : meshes) {
            piece.draw();
        }
    }

private:
    Meshes meshes;
};

#endif /* OBJMODEL_H */

