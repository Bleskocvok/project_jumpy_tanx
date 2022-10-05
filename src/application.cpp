#include <iostream>

#include "application.hpp"

#include <stb_image.h>

#include <math.h>

Application::Application(size_t initial_width, size_t initial_height)
        : width(initial_width)
        , height(initial_height)
        , post(initial_width, initial_height) {

    glViewport(0, 0, width, height);
    post.clearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // post.clearColor(0.3, 0.3, 0.3, 1.0);

    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    text_renderer.resize(width, height);

    default_mat.set_ambient(0.2, 0.2, 0.2)
            .set_diffuse(0.5, 0.5, 0.5)
            .set_specular(1.0, 1.0, 1.0)
            .set_shine(100.0);

    terrain_mat.set_specular(0.0, 0.0, 0.0);

    add_enemy(glm::vec3(30.0, 5.0, 30.0));
    add_random_enemy();
}

void Application::add_random_enemy() {
    float x = rand.get_float(terrain.x_min(), terrain.x_max());
    float z = rand.get_float(terrain.z_min(), terrain.z_max());
    glm::vec3 position{
        x,
        terrain.get_height(x, z) + 5.0,
        z
    };
    add_enemy(position);
}

void Application::update(double time) {
    if (paused) {
        return;
    }

    explosions.clear();

    /*
     * Player tank.
     */
    if (!tank.dead()) {
        if (button_up) {
            tank.forward(0.01f);
        }
        if (button_down) {
            tank.forward(-0.01f);
        }
        if (button_left) {
            tank.turn(0.01f);
        }
        if (button_right) {
            tank.turn(-0.01f);
        }
        if (cannon_up) {
            tank.move_cannon(time * 2.0);
        }
        if (cannon_down) {
            tank.move_cannon(-time * 2.0);
        }
        tank.set_cannon_rotation(camera.get_rotation_y());
    }
    tank.update(time);

    camera.set_focus(tank.get_mass_center() + glm::vec3(0.0f, 4.0f, 0.0f));

    projectiles.update(time);
    particles.update(time);

    lights.update();

    bool player_dead_before = tank.dead();
    explosions.calc_tank(tank);
    if (tank.dead() && !player_dead_before) {
        particles.tank_explosion(tank.get_mass_center(), 0.9);
        particles.tank_explosion(tank.get_mass_center(), 0.9);
        particles.tank_explosion(tank.get_mass_center(), 0.9);
        particles.tank_explosion(tank.get_mass_center(), 0.9);
    }

    /*
     * Update enemies.
     */
    for (size_t i = 0; i < enemies.size(); ++i) {
        auto &enemy_ptr = enemies[i];
        enemy_ptr->aim(tank.get_mass_center());
        enemy_ptr->update(time);
        explosions.calc_tank(enemy_ptr->tank);
        if (enemy_ptr->tank.squash(tank)) {
            score += 2; // 3 points total for squashing
        }
        enemy_ptr->tank.push(tank);

        if (enemy_ptr->tank.dead()) {
            particles.tank_explosion(enemy_ptr->tank.get_mass_center());
            enemies.erase(enemies.begin() + i--);
            ++score;
        }
        for (size_t k = 0; k < enemies.size(); ++k) {
            if (i < k) {
                enemy_ptr->tank.push(enemies[k]->tank);
            }
        }
    }

    if (spawn_timer >= ENEMY_SPAWN_TIME) {
            spawn_timer -= ENEMY_SPAWN_TIME;
            if (enemies.size() < MAX_ENEMY_COUNT) {
                    add_random_enemy();
            }
    }
    spawn_timer += time;

    /*
     * Contiunuous shooting, baby!
     */
    shoot_countdown -= time;
    if (shoot_countdown <= 0.0) {
        shoot_countdown = 0.0;
        if (shooting) {
            if (!tank.dead()) {
                projectiles.add(tank.get_mass_center(), glm::normalize(tank.get_cannon_vector()));
            }
            shoot_countdown = SHOOT_TIME;
        }
    }
}

void Application::render(double time) {
    /*
     * Send camera matrices.
     */
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 115.0f);
    glm::mat4 model_matrix = glm::mat4(1.0f);

    glProgramUniformMatrix4fv(main_program.index(), Locs::PROJ, 1, false, glm::value_ptr(projection_matrix));
    glProgramUniformMatrix4fv(main_program.index(), Locs::MODEL, 1, false, glm::value_ptr(model_matrix));
    glProgramUniformMatrix4fv(main_program.index(), Locs::VIEW, 1, false, glm::value_ptr(camera.view_matrix()));
    glProgramUniform3f(main_program.index(), 3, camera.eye_pos().x, camera.eye_pos().y, camera.eye_pos().z);

    default_mat.send(main_program);

    /*
     * Draw whole scene.
     */
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    post.bind();

    main_program.use();
    lights.send(main_program);

    if (started) {
        for (auto &enemy_ptr : enemies) {
            enemy_ptr->draw(main_program, time);
        }
        tank.draw(main_program);
    }
    projectiles.draw(main_program, time);
    particles.draw(main_program, time);
    trees.draw(main_program);
    terrainModel.draw(main_program);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    post.draw();

    text_renderer.draw_text("score: " + std::to_string(score), -0.9, 0.8, 0.03);
    text_renderer.draw_text(paused ? "ESC or MOUSE CLICK: unpause" : "ESC: pause", -0.9, 0.72, 0.02);
    if (tank.dead()) {
        text_renderer.draw_text("ur tank boom", -0.5, 0.4, 0.1);
        text_renderer.draw_text("game over - ENTER to play again", -0.5, 0.2, 0.03);
    } else if (paused) {
        if (started) {
            text_renderer.draw_text("PAUSED", -0.1, 0.8, 0.05);
        } else {
            text_renderer.draw_text("Jumpy Tanx", -0.2, 0.8, 0.1);
            text_renderer.draw_text("pv112 - project", -0.2, 0.65, 0.03);
        }
        text_renderer.draw_text("WSAD - movement", -0.8, 0.3, 0.03);
        text_renderer.draw_text("LEFT MOUSE BUTTON - shoot", -0.8, 0.2, 0.03);
        text_renderer.draw_text("SPACEBAR - jump!!! - squash your enemies", -0.8, 0.1, 0.03);
        text_renderer.draw_text("Q / E or MOUSE WHEEL - change cannon rotation", -0.8, 0.0, 0.03);
        text_renderer.draw_text("shoot or squash your enemies to destroy them", -0.6, -0.4, 0.03);
    }
}

void Application::on_scroll(GLFWwindow*, double, double offset) {
    tank.move_cannon(-offset * 0.1);
}

void Application::on_resize(GLFWwindow*, int new_width, int new_height) {
    width = new_width;
    height = new_height;

    glViewport(0, 0, width, height);

    post.resize(width, height);
    text_renderer.resize(width, height);
}

void Application::on_mouse_move(GLFWwindow*, double x, double y) {
    if (moving_view && !paused) {
        double diff_y = y - prev_mouse_y;
        double diff_x = x - prev_mouse_x;
        double sens_y = 0.003;
        double sens_x = -0.003;
        camera.rotate(diff_x * sens_x, diff_y * sens_y);
    }
    prev_mouse_x = x;
    prev_mouse_y = y;
}

void Application::on_mouse_pressed(GLFWwindow* window, int button, int action, int /*mods*/) {
    if (paused) {
        paused = false;
        started = true;
        glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        return;
    }
//        if (!tank.dead()) {
//                if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//                        projectiles.add(tank.get_mass_center(), glm::normalize(tank.get_cannon_vector()));
//                }
//        }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            shooting = true;
        }
        if (action == GLFW_RELEASE) {
            shooting = false;
        }
    }
}

void Application::on_key_pressed(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            button_up = true;
        } else if (action == GLFW_RELEASE) {
            button_up = false;
        }
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            button_down = true;
        } else if (action == GLFW_RELEASE) {
            button_down = false;
        }
    }
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            button_left = true;
        } else if (action == GLFW_RELEASE) {
            button_left = false;
        }
    }
    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            button_right = true;
        } else if (action == GLFW_RELEASE) {
            button_right = false;
        }
    }
    if (key == GLFW_KEY_E) {
        if (action == GLFW_PRESS) {
            cannon_up = true;
        } else if (action == GLFW_RELEASE) {
            cannon_up = false;
        }
    }
    if (key == GLFW_KEY_Q) {
        if (action == GLFW_PRESS) {
            cannon_down = true;
        } else if (action == GLFW_RELEASE) {
            cannon_down = false;
        }
    }
    if (key == GLFW_KEY_ESCAPE) {
        if (action == GLFW_PRESS) {
            paused = !paused;
            started = true;
            glfwSetInputMode(window, GLFW_CURSOR, paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        started = true;
        if (paused) {
            paused = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        if (tank.dead()) {
            quit = true;
            play = true;
        }
    }
    if (!tank.dead()) {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            tank.jump();
        }
    }
}
