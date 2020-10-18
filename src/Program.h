
/*
 * File:   Program.h
 * Author: Frantisek Brablik
 *
 * Created on 12. dubna 2019, 22:59
 */

#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

#include "utils/graphics.h"

#include <string>
#include <vector>
#include "utils/utils.hpp"


class Program;

class Shader {
public:
    Shader(GLenum type, const std::string& file) {
        index = glCreateShader(type);
        std::string content = utils::load_file(file);
        const char* fileContent = content.data();
        glShaderSource(index, 1, &fileContent, nullptr);
        glCompileShader(index);

        GLint compiled = 0;
        glGetShaderiv(index, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(index, GL_INFO_LOG_LENGTH, &length);
            std::vector<GLchar> error_msg(length);
            glGetShaderInfoLog(index, length, &length, &error_msg[0]);
            glDeleteShader(index);
            std::cout << "SHADER '" << file << "' ERROR: " << error_msg.data() << "\n";
            throw std::runtime_error("shader compilation error");
        }

    }

    ~Shader() { glDeleteShader(index); }

private:
    GLuint index;
    friend Program;
};

class Program {
public:
    Program(const std::string& vert, const std::string& frag)
        : Program(Shader(GL_VERTEX_SHADER, vert), Shader(GL_FRAGMENT_SHADER, frag)) {}

    Program(const Shader& vert, const Shader& frag) {
        program = glCreateProgram();
        glAttachShader(program, vert.index);
        glAttachShader(program, frag.index);
        glLinkProgram(program);
    }

    ~Program() { glDeleteProgram(program); }

    void use() const { glUseProgram(program); }
    GLuint index() const { return program; }

private:
    GLuint program{};
};

#endif /* PROGRAM_H */

