cmake_minimum_required(VERSION 3.18)

project(JumpyTanx)


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


#
# General settings
#
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})


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


#
# GLFW
#
add_subdirectory(libraries/glfw-3.3.2)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


#
# stb_image
#
include_directories(libraries/stb_image)
add_library(stb_image libraries/stb_image/stb_image.c)


#
# tiny_obj_loader
#
include_directories(libraries/tiny_obj_loader)
add_library(tiny_obj_loader libraries/tiny_obj_loader/tiny_obj_loader.cpp)



# ################################################
#
# ---------------- The executable ----------------
#
# ################################################


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -std=c++17 \
    -Wall \
    -Wextra \
    -pedantic \
    -O2 \
")

add_executable(jumpy_tanx WIN32 ${SRC_FILES})

target_link_libraries(jumpy_tanx
    glfw
    glad
    stb_image
    tiny_obj_loader
    ${OPENGL_LIBRARIES}
    )




# ################################################
#
# ------------- Files configurations -------------
#
# ################################################


#
# Shaders
#

set(SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/shaders)

configure_file(${SHADERS}/main.vert ${SHADERS_BINARY}/main.vert COPYONLY)
configure_file(${SHADERS}/main.frag ${SHADERS_BINARY}/main.frag COPYONLY)

configure_file(${SHADERS}/post_main.vert ${SHADERS_BINARY}/post_main.vert COPYONLY)
configure_file(${SHADERS}/blur_horizontal.frag ${SHADERS_BINARY}/blur_horizontal.frag COPYONLY)
configure_file(${SHADERS}/blur_vertical.frag ${SHADERS_BINARY}/blur_vertical.frag COPYONLY)
configure_file(${SHADERS}/bright.frag ${SHADERS_BINARY}/bright.frag COPYONLY)
configure_file(${SHADERS}/add.frag ${SHADERS_BINARY}/add.frag COPYONLY)

configure_file(${SHADERS}/text.vert ${SHADERS_BINARY}/text.vert COPYONLY)
configure_file(${SHADERS}/text.frag ${SHADERS_BINARY}/text.frag COPYONLY)


#
# Models
#

set(MODELS ${CMAKE_CURRENT_SOURCE_DIR}/models)
set(MODELS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/models)

configure_file(${MODELS}/green.png ${MODELS_BINARY}/green.png COPYONLY)

configure_file(${MODELS}/terrain_texture.png ${MODELS_BINARY}/terrain_texture.png COPYONLY)
configure_file(${MODELS}/tank_light_map.png ${MODELS_BINARY}/tank_light_map.png COPYONLY)

configure_file(${MODELS}/tank_torso.obj ${MODELS_BINARY}/tank_torso.obj COPYONLY)
configure_file(${MODELS}/tank_head.obj ${MODELS_BINARY}/tank_head.obj COPYONLY)
configure_file(${MODELS}/tank_cannon.obj ${MODELS_BINARY}/tank_cannon.obj COPYONLY)

configure_file(${MODELS}/tank_texture.png ${MODELS_BINARY}/tank_texture.png COPYONLY)
configure_file(${MODELS}/tank_light_map.png ${MODELS_BINARY}/tank_light_map.png COPYONLY)

configure_file(${MODELS}/dirt_1_texture.png ${MODELS_BINARY}/dirt_1_texture.png COPYONLY)
configure_file(${MODELS}/neutral_light_map.png ${MODELS_BINARY}/neutral_light_map.png COPYONLY)
configure_file(${MODELS}/dirt_1.obj ${MODELS_BINARY}/dirt_1.obj COPYONLY)

configure_file(${MODELS}/ball.obj ${MODELS_BINARY}/ball.obj COPYONLY)
configure_file(${MODELS}/ball_texture.png ${MODELS_BINARY}/ball_texture.png COPYONLY)
configure_file(${MODELS}/ball_light_map.png ${MODELS_BINARY}/ball_light_map.png COPYONLY)

configure_file(${MODELS}/beautifont.png ${MODELS_BINARY}/beautifont.png COPYONLY)

configure_file(${MODELS}/player_tank_torso.obj ${MODELS_BINARY}/player_tank_torso.obj COPYONLY)
configure_file(${MODELS}/player_tank_head.obj ${MODELS_BINARY}/player_tank_head.obj COPYONLY)
configure_file(${MODELS}/player_tank_cannon.obj ${MODELS_BINARY}/player_tank_cannon.obj COPYONLY)

configure_file(${MODELS}/player_tank_torso_texture.png ${MODELS_BINARY}/player_tank_torso_texture.png COPYONLY)
configure_file(${MODELS}/player_tank_head_texture.png ${MODELS_BINARY}/player_tank_head_texture.png COPYONLY)
configure_file(${MODELS}/player_tank_cannon_texture.png ${MODELS_BINARY}/player_tank_cannon_texture.png COPYONLY)
configure_file(${MODELS}/player_tank_torso_light_map.png ${MODELS_BINARY}/player_tank_torso_light_map.png COPYONLY)

configure_file(${MODELS}/tank_particle.obj ${MODELS_BINARY}/tank_particle.obj COPYONLY)
configure_file(${MODELS}/tank_particle_texture.png ${MODELS_BINARY}/tank_particle_texture.png COPYONLY)

configure_file(${MODELS}/explosion.obj ${MODELS_BINARY}/explosion.obj COPYONLY)
configure_file(${MODELS}/explosion_texture.png ${MODELS_BINARY}/explosion_texture.png COPYONLY)

configure_file(${MODELS}/tree.obj ${MODELS_BINARY}/tree.obj COPYONLY)
configure_file(${MODELS}/tree_texture.png ${MODELS_BINARY}/tree_texture.png COPYONLY)
configure_file(${MODELS}/tree_light_map.png ${MODELS_BINARY}/tree_light_map.png COPYONLY)

configure_file(${MODELS}/light_bush.obj ${MODELS_BINARY}/light_bush.obj COPYONLY)
configure_file(${MODELS}/light_bush_texture.png ${MODELS_BINARY}/light_bush_texture.png COPYONLY)
configure_file(${MODELS}/light_bush_light_map.png ${MODELS_BINARY}/light_bush_light_map.png COPYONLY)

