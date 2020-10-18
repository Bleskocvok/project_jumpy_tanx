
/*
 * File:   RandGen.h
 * Author: Frantisek Brablik
 *
 * Created on 2. května 2019, 14:13
 */

#pragma once

#ifndef RANDGEN_H
#define RANDGEN_H

#include <random>
#include <ctime>

class RandGen {
public:
    RandGen()
            : gen(std::time(nullptr)) {}
    RandGen(unsigned seed)
            : gen(seed) {}
   
    float get_float(float min_val, float max_val) {
      if (min_val >= max_val) {
        return min_val;
	  }
      if (min_val <= max_val) {
			std::uniform_real_distribution<> dist(min_val, max_val);
			return dist(gen);
		}
      return min_val;
    }
   
    int get_int(int min, int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
private:
    std::default_random_engine gen;
};

#endif /* RANDGEN_H */

