#pragma once

#include "utils/graphics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "FBO.h"
#include "Program.h"
#include "Texture.h"
#include "ObjModel.h"
#include "Material.h"
#include "TankMatrix.h"
#include "TerrainModel.h"

#include "Tank.h"
#include "Camera.h"

#include "Projectiles.h"
#include "Particles.h"

#include "Lights.h"
#include "Enemy.h"

#include "Explosions.h"

#include "Postprocess.h"

#include "TextRenderer.h"

#include "Trees.h"

class Application {
public:
    Application(size_t initial_width, size_t initial_height);

    void update(double time);
    void render(double time);

    /*
     * Event callbacks.
     */
    void on_resize(GLFWwindow* window, int width, int height);
    void on_mouse_move(GLFWwindow* window, double x, double y);
    void on_mouse_pressed(GLFWwindow* window, int button, int action, int mods);
    void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void on_scroll(GLFWwindow* window, double, double offset);

    bool has_quit() const { return quit; }

    bool play_again() const { return play; }

    size_t get_width() const { return width; }
    size_t get_height() const { return height; }

private:
    void add_enemy(glm::vec3 pos, unsigned type = 0) {
        enemies.emplace_back(std::make_unique<NormalEnemy>(
                &terrain,
                pos,
                glm::vec3(1.4f, 1.5f, 2.4f),
                ModelRef(&enemy1_tank_torso_model, &default_mat, &enemy1_tank_texture, &enemy1_tank_light_map),
                ModelRef(&enemy1_tank_head_model, nullptr, nullptr, nullptr),
                ModelRef(&enemy1_tank_cannon_model, nullptr, nullptr, nullptr),
                projectiles));
    }

    void add_random_enemy();

    /*
     * Attributes.
     */
    size_t width;
    size_t height;

    bool quit = false;
    bool play = false;

    /*
     * Enemy spawns.
     */
    unsigned score = 0;
    static constexpr float ENEMY_SPAWN_TIME = 3.73;
    static constexpr unsigned MAX_ENEMY_COUNT = 6;
    float spawn_timer = 0.0;
    RandGen rand;

    /*
     * General whings.
     */
    Program main_program = Program("shaders/main.vert", "shaders/main.frag");

    Camera camera{ 16.0f };

    Terrain terrain{ 1, 7.0, 140, 8 };
    TerrainModel terrainModel{
        terrain,
        1.5 * 0.5,
        ModelRef(nullptr, &terrain_mat, &terrain_tex, nullptr)
    };

    Lights lights;

    Explosions explosions{ 5.0, 160 };

    Particles particles{ &terrain };
    Projectiles projectiles{ terrain, particles, explosions, lights, 0.3 };

    Postprocess post;

    TextRenderer text_renderer{
        "models/beautifont.png",
        16,
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 .,!?-:/_'#*\"+abcdefghijklmnopqrstuvwxyz=()",
        1.8
    };


    Trees trees{ terrain, lights, 1 };

    /*
     * Controls.
     */
    static constexpr float SHOOT_TIME = 0.2;
    float shoot_countdown = SHOOT_TIME;
    bool shooting = false;

    bool cannon_up = false;
    bool cannon_down = false;

    bool button_up = false;
    bool button_down = false;
    bool button_left = false;
    bool button_right = false;

    double prev_mouse_x = 0;
    double prev_mouse_y = 0;
    bool moving_view = true;

    bool paused = true;
    bool started = false;

    /*
     * Enemies.
     */
    std::vector<std::unique_ptr<Enemy>> enemies;


    /*
     * SPECIFIL MODELS
     */
    Material terrain_mat{ true };
    Texture terrain_tex{ "models/terrain_texture.png" };
    Texture terrain_light{ "models/tank_light_map.png" };

    ObjModel tank_torso_model{ "models/player_tank_torso.obj" };
    ObjModel tank_head_model{ "models/player_tank_head.obj" };
    ObjModel tank_cannon_model{ "models/player_tank_cannon.obj" };

    Material default_mat = Material(Locs::MAT_AMB, Locs::MAT_DIF, Locs::MAT_SPEC, Locs::MAT_SHINE);

    Texture tank_torso_texture = Texture("models/player_tank_torso_texture.png");
    Texture tank_head_texture = Texture("models/player_tank_head_texture.png");
    Texture tank_cannon_texture = Texture("models/player_tank_cannon_texture.png");
    Texture tank_torso_light_map = Texture("models/player_tank_torso_light_map.png");

    Tank tank{ &terrain,
            glm::vec3((terrain.x_max() + terrain.x_min()) * 0.5, 3.0f, (terrain.z_max() + terrain.z_min()) * 0.5),
            glm::vec3(1.4f, 1.5f, 2.4f),
            ModelRef(&tank_torso_model, &default_mat, &tank_torso_texture, &tank_torso_light_map),
            ModelRef(&tank_head_model, nullptr, &tank_head_texture, nullptr),
            ModelRef(&tank_cannon_model, nullptr, &tank_cannon_texture, nullptr)
    };

    /*
     * Enemy tanks.
     */
    ObjModel enemy1_tank_torso_model{ "models/tank_torso.obj" };
    ObjModel enemy1_tank_head_model{ "models/tank_head.obj" };
    ObjModel enemy1_tank_cannon_model{ "models/tank_cannon.obj" };
    Texture enemy1_tank_texture{ "models/tank_texture.png" };
    Texture enemy1_tank_light_map = Texture("models/tank_light_map.png");
};
