
/*
 * File:   Terrain.h
 * Author: Frantisek Brablik
 *
 * Created on 19. dubna 2019, 21:43
 */

#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include <random>
#include <vector>
#include <cmath>
#include <map>
#include <utility>

#ifndef M_PI
#define M_PI 3.14159
#endif

class Terrain {
public:
    Terrain(unsigned seed, float amplitude, unsigned map_size, int smoothness = 1)
            : height_map(map_size + 2, std::vector<float>(map_size + 2))
            , map_size(map_size)
            , smoothness(smoothness) {

        for (size_t z = 0; z < map_size; z += 1) {
            for (size_t x = 0; x < map_size; x += 1) {
                //height_map[z][x] = get_blurred(x, z, copy);
                height_map[z][x] = gen_height(seed, x, z, amplitude);
            }
        }
        gen_memory.clear();
    }
    float get_height(float x, float z) const {
        x /= smoothness;
        z /= smoothness;
//        if (x < 0.0 || z < 0.0 || x >= x_max() || z >= z_max()) {
//            return 0.0;
//        }
        int int_x = x;
        int int_z = z;
        if (int_x < 0
                || int_z < 0
                || static_cast<unsigned>(int_x) >= map_size
                || static_cast<unsigned>(int_z) >= map_size) {
            return 0.0;
        }
        float diff_x = x - int_x;
        float diff_z = z - int_z;
        float one_x = interpolate(height_map[int_z][int_x], height_map[int_z][int_x + 1], diff_x);
        float two_x = interpolate(height_map[int_z + 1][int_x], height_map[int_z + 1][int_x + 1], diff_x);
        return interpolate(one_x, two_x, diff_z);
    }
    unsigned size() const {
        return map_size;
    }
    float x_min() const {
        return 0.0;
    }
    float z_min() const {
        return 0.0;
    }
    float x_max() const {
        return map_size;
    }
    float z_max() const {
        return map_size;
    }
private:
    std::vector<std::vector<float>> height_map;
    unsigned map_size;
    float smoothness;
    std::map<std::pair<int, int>, float> gen_memory;


    float gen_height(unsigned seed, float x, float z, float amplitude) {
        unsigned int_x = x;
        unsigned int_z = z;
        float diff_x = x - int_x;
        float diff_z = z - int_z;
        float one_x = interpolate(gen_smooth_height(seed, int_z, int_x, amplitude), gen_smooth_height(seed, int_z, int_x + 1, amplitude), diff_x);
        float two_x = interpolate(gen_smooth_height(seed, int_z + 1, int_x, amplitude), gen_smooth_height(seed, int_z + 1, int_x + 1, amplitude), diff_x);
        return interpolate(one_x, two_x, diff_z);
    }
    float get_blurred(unsigned x, unsigned z, std::vector<std::vector<float>>& src) const {
        if (x == 0 || z == 0 || x == map_size - 1 || z == map_size - 1) {
            return src[z][x];
        }
        float corners = src[z - 1][x - 1] + src[z - 1][x + 1] + src[z + 1][x - 1] + src[z + 1][x + 1];
        float sides = src[z][x - 1] + src[z][x + 1] + src[z - 1][x] + src[z + 1][x];
        return src[z][x] * 0.25 + sides * 0.125 + corners / 16.0;
    }

    float interpolate(float one, float two, float coef) const {
        float height = (1.0f - cos(coef * M_PI)) * 0.5f;
        return two * height + (1.0f - height) * one;
    }

    float gen_smooth_height(unsigned seed, int x, int z, float amplitude) {
        float corners = gen_num(seed, x - 1, z - 1, amplitude)
                + gen_num(seed, x + 1, z - 1, amplitude)
                + gen_num(seed, x + 1, z + 1, amplitude)
                + gen_num(seed, x - 1, z + 1, amplitude);
        float sides = gen_num(seed, x - 1, z, amplitude)
                + gen_num(seed, x + 1, z, amplitude)
                + gen_num(seed, x, z - 1, amplitude)
                + gen_num(seed, x, z + 1, amplitude);
        return gen_num(seed, x, z, amplitude) * 0.25 + sides * 0.125 + corners / 16.0;
    }

    float gen_num(unsigned seed, int x, int z, float amplitude) {
        auto found = gen_memory.find({ x, z });
        if (found != gen_memory.end()) {
            return found->second;
        }
        std::mt19937 generator(seed + x * 49831 + z * 785312); //mt19937 || default_random_engine
        std::uniform_real_distribution<> distribute(-amplitude, amplitude);
        float result = distribute(generator);
        gen_memory.emplace(std::make_pair(x, z), result);
        return result;
    }
};

#endif /* TERRAIN_H */

