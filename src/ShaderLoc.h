
/*
 * File:   ShaderLoc.h
 * Author: Frantisek Brablik
 *
 * Created on 20. dubna 2019, 23:58
 */

#pragma once

#ifndef SHADERLOC_H
#define SHADERLOC_H

#include "utils/graphics.h"

class Locs {
public:
    static const GLint PROJ = 0;
    static const GLint VIEW = 2;
    static const GLint MODEL = 1;

    static const GLint LIGHT_POS = 4;
    static const GLint LIGHT_AMB = 5;
    static const GLint LIGHT_DIF = 6;
    static const GLint LIGHT_SPEC = 7;

    static const GLint MAT_AMB = 8;
    static const GLint MAT_DIF = 9;
    static const GLint MAT_SPEC = 10;
    static const GLint MAT_SHINE = 11;

    static const GLint DIF_TEXTURE = 0;
    static const GLint LIGHT_TEXTURE = 1;
    static const GLint USE_LIGHT_TEX = 12;
};

#endif /* SHADERLOC_H */

