#!/bin/sh
cmake -G "Unix Makefiles" -B . -S . && cmake --build . -j4
