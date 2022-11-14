#!/bin/sh
cmake -G "Unix Makefiles" -B bin-lin -S . && cmake --build bin-lin -j4
