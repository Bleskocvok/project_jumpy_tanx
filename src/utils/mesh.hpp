#pragma once

#include "graphics.h"

#include <tiny_obj_loader.h>

#include <memory>
#include <vector>
#include <cstdint>
#include <exception>
#include <iostream>


/*
 * VAO
 */

class VAO {
public:
    VAO() {
        glCreateVertexArrays(1, &buffer);
    }

    ~VAO() {
        glDeleteVertexArrays(1, &buffer);
    }

    VAO(const VAO&) = delete;
    VAO& operator=(const VAO&) = delete;

    VAO(VAO&& other) noexcept {
        buffer = other.buffer;
        other.buffer = 0;
    }
    VAO& operator=(VAO&& other) noexcept {
        buffer = other.buffer;
        other.buffer = 0;
        return *this;
    }

    GLuint get() {
        return buffer;
    }

    void bind() {
        glBindVertexArray(buffer);
    }

private:
    GLuint buffer = 0;
};


/*
 * VBO
 */

template<int Dimension = 3>
class VBO {
public:
    VBO(const std::vector<float>& data) {
        if (!data.empty()) {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(
                    buffer,
                    data.size() * sizeof(float),
                    data.data(),
                    GL_DYNAMIC_STORAGE_BIT);
        }
    }

    VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept {
        buffer = other.buffer;
        other.buffer = 0;
    }
    VBO& operator=(VBO&& other) noexcept {
        buffer = other.buffer;
        other.buffer = 0;
        return *this;
    }

    ~VBO() {
        glDeleteBuffers(1, &buffer);
    }

    void assign(VAO& vao, GLint location) {
        glVertexArrayVertexBuffer(
                vao.get(),
                location,
                buffer,
                0,
                Dimension * sizeof(float));
        glEnableVertexArrayAttrib(
                vao.get(),
                location);
        glVertexArrayAttribFormat(
                vao.get(),
                location,
                Dimension,
                GL_FLOAT,
                GL_FALSE,
                0);
        glVertexArrayAttribBinding(
                vao.get(),
                location,
                location);
    }

private:
    GLuint buffer = 0;
};


/*
 * VBOIndices
 */

class VBOIndices {
public:
    VBOIndices(const std::vector<uint32_t>& data) {
        if (!data.empty()) {
            glCreateBuffers(1, &buffer);
            glNamedBufferStorage(
                    buffer,
                    data.size() * sizeof(uint32_t),
                    data.data(),
                    GL_DYNAMIC_STORAGE_BIT);
        }
        count = data.size();
    }

    VBOIndices(const VBOIndices&) = delete;
    VBOIndices& operator=(const VBOIndices&) = delete;

    VBOIndices(VBOIndices&& other) noexcept {
        buffer = other.buffer;
        count = other.count;
        other.buffer = 0;
        other.count = 0;
    }
    VBOIndices& operator=(VBOIndices&& other) noexcept {
        buffer = other.buffer;
        count = other.count;
        other.buffer = 0;
        other.count = 0;
        return *this;
    }

    ~VBOIndices() {
        glDeleteBuffers(1, &buffer);
    }

    void assign(VAO& vao) {
        glVertexArrayElementBuffer(vao.get(), buffer);
    }

    bool good() const {
        return buffer > 0;
    }

private:
    GLuint buffer = 0;
    int count = 0;
};


/*
 * Mesh,
 * Meshes
 *
 * The main classes.
 */

class Mesh;

using Meshes = std::vector<Mesh>;

class Mesh {

inline static GLenum Mode = GL_TRIANGLES;
inline static GLint VerticesLocation = 0;
inline static GLint NormalsLocation = 1;
inline static GLint TexCoordsLocation = 2;

public:
    Mesh(std::vector<float> vertices_buf
            , std::vector<float> normals_buf
            , std::vector<float> tex_coords_buf
            , std::vector<uint32_t> indices_buf)
            : vertices(vertices_buf)
            , normals(normals_buf)
            , tex_coords(tex_coords_buf)
            , indices(indices_buf) {
        // need to remember these sizes
        vertices_count = vertices_buf.size() / 3;
        indices_count = indices_buf.size();
        // assigns
        vertices.assign(vao, VerticesLocation);
        normals.assign(vao, NormalsLocation);
        tex_coords.assign(vao, TexCoordsLocation);
        indices.assign(vao);
    }

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) = default;
    Mesh& operator=(Mesh&&) = default;

    ~Mesh() = default;

    void draw() {
        vao.bind();
        if (indices.good()) {
            // draw with indices
            glDrawElements(Mode, indices_count, GL_UNSIGNED_INT, nullptr);
        } else {
            // otherwise draw raw
            glDrawArrays(Mode, 0, static_cast<GLsizei>(vertices_count));
        }
    }

    static Meshes load_file(const std::string& filename) {
        tinyobj::attrib_t attr;
        std::vector<tinyobj::shape_t> shapes;
        std::string warnings;
        std::string errors;
        bool ok = tinyobj::LoadObj(
                &attr,
                &shapes,
                nullptr,            // materials (not needed)
                &warnings,
                &errors,
                filename.c_str(),
                nullptr,            // folder for materials (not needed)
                true                // triangulate = true
                );
        if (!ok) {
            std::cout << "WARNING: " << warnings << "\n"
                    << "ERROR: " << errors << std::endl;
            throw std::runtime_error("could not load model, file '" + filename + "'");
        }
        Meshes result;
        for (const auto& shape : shapes) {
            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<float> tex_coords;
            unsigned size = shape.mesh.num_face_vertices.size();
            unsigned triangle = 3;  // a dumb constant to make the code more readable; means the number of points in a triangle
            vertices.reserve(size * triangle * 3);
            normals.reserve(size * triangle * 3);
            tex_coords.reserve(size * triangle * 2);
            for (unsigned i = 0; i < size; ++i) {
                for (unsigned j = 0; j < triangle; ++j) {
                    tinyobj::index_t index = shape.mesh.indices[i * 3 + j];
                    // vertices
                    vertices.push_back(attr.vertices[3 * index.vertex_index + 0]);
                    vertices.push_back(attr.vertices[3 * index.vertex_index + 1]);
                    vertices.push_back(attr.vertices[3 * index.vertex_index + 2]);
                    // normals
                    normals.push_back(attr.normals[3 * index.normal_index + 0]);
                    normals.push_back(attr.normals[3 * index.normal_index + 1]);
                    normals.push_back(attr.normals[3 * index.normal_index + 2]);
                    // texture coords
                    tex_coords.push_back(attr.texcoords[2 * index.texcoord_index + 0]);
                    tex_coords.push_back(attr.texcoords[2 * index.texcoord_index + 1]);
                }
            }
            result.push_back(
                Mesh(vertices, normals, tex_coords, std::vector<uint32_t>{}));
        }
        return result;
    }

private:
    VAO vao{};

    VBO<3> vertices;
    VBO<3> normals;
    VBO<2> tex_coords;
    VBOIndices indices;

    int vertices_count = 0;
    int indices_count = 0;
};
