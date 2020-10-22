cmake_minimum_required(VERSION 3.18)

project(JumpyTanx)


#
# Things I needed to install on linux:
#

# sudo apt install libgl1-mesa-dev
# sudo apt install mesa-common-dev

# sudo apt install libxrandr-dev
#   libxinerama-dev
#   libxcursor-dev
#   libxi-dev


#
# Source files
#
set(SRC_FILES
    src/main.cpp
    src/application.cpp
)



# ################################################
#
# --------------- Libraries setup ----------------
#
# ################################################

set(LIST_OF_LIBRARIES "")

macro(add_lib_to_list VALUE)
    list(APPEND LIST_OF_LIBRARIES "${VALUE}")
endmacro()

#
# General settings
#
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
add_lib_to_list("${OPENGL_LIBRARIES}")

#
# GLM
#
include_directories(libraries/glm-0.9.9.8/glm)
add_subdirectory(libraries/glm-0.9.9.8/glm)


#
# Glad
#
add_library(glad libraries/glad/src/glad.c)
set(GLAD_INCLUDE_DIRS libraries/glad/include)
include_directories(${GLAD_INCLUDE_DIRS})
add_lib_to_list(glad)


#
# GLFW
#
add_subdirectory(libraries/glfw-3.3.2)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_lib_to_list(glfw)


#
# stb_image
#
include_directories(libraries/stb_image)
add_library(stb_image libraries/stb_image/stb_image.c)
add_lib_to_list(stb_image)


#
# tiny_obj_loader
#
include_directories(libraries/tiny_obj_loader)
add_library(tiny_obj_loader libraries/tiny_obj_loader/tiny_obj_loader.cpp)
add_lib_to_list(tiny_obj_loader)



# ################################################
#
# ---------------- The executable ----------------
#
# ################################################

message(STATUS "Libraries: ${LIST_OF_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -std=c++17 \
    -Wall \
    -Wextra \
    -pedantic \
    -O2 \
")

add_executable(jumpy_tanx WIN32 ${SRC_FILES})

target_link_libraries(jumpy_tanx ${LIST_OF_LIBRARIES})




# ################################################
#
# ------------- Files configurations -------------
#
# ################################################

function(asset FILENAME)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COPYONLY
    )
endfunction()


#
# Shaders
#

asset(shaders/main.vert)
asset(shaders/main.frag)

asset(shaders/post_main.vert)
asset(shaders/blur_horizontal.frag)
asset(shaders/blur_vertical.frag)
asset(shaders/bright.frag)
asset(shaders/add.frag)

asset(shaders/text.vert)
asset(shaders/text.frag)


#
# Models
#

asset(models/green.png)

asset(models/terrain_texture.png)
asset(models/tank_light_map.png)

asset(models/tank_torso.obj)
asset(models/tank_head.obj)
asset(models/tank_cannon.obj)

asset(models/tank_texture.png)
asset(models/tank_light_map.png)

asset(models/dirt_1_texture.png)
asset(models/neutral_light_map.png)
asset(models/dirt_1.obj)

asset(models/ball.obj)
asset(models/ball_texture.png)
asset(models/ball_light_map.png)

asset(models/beautifont.png)

asset(models/player_tank_torso.obj)
asset(models/player_tank_head.obj)
asset(models/player_tank_cannon.obj)

asset(models/player_tank_torso_texture.png)
asset(models/player_tank_head_texture.png)
asset(models/player_tank_cannon_texture.png)
asset(models/player_tank_torso_light_map.png)

asset(models/tank_particle.obj)
asset(models/tank_particle_texture.png)

asset(models/explosion.obj)
asset(models/explosion_texture.png)

asset(models/tree.obj)
asset(models/tree_texture.png)
asset(models/tree_light_map.png)

asset(models/light_bush.obj)
asset(models/light_bush_texture.png)
asset(models/light_bush_light_map.png)

