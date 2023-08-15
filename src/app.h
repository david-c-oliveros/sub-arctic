#ifndef APP_H
#define APP_H

#include <iostream>
#include <math.h>
#include <memory>

#include <glad.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h> /* https://github.com/glfw/glfw */

#include "gltext.h" /* https://github.com/vallentin/glText */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "stb_image.h"
#include "miniaudio.h"

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "object.h"
#include "player.h"
#include "box_collider.h"
#include "timer.h"
#include "particle_system.h"


#define GRAVITY -0.001f
#define FRICTION 0.99f
#define THRUST   0.01f


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

std::vector<glm::vec3> load_position_data(const char* filepath);



enum class Game_State
{
    MENU,
    RUNNING,
    LOSE,
    WIN,
    RESTART
};



struct Pos_Vel_Rot
{
    glm::vec3 pos;
    glm::vec3 vel;
    float rot_angle;
    bool anchored;
    bool buoyant;
    glm::vec3 torpedo_initial_impulse = glm::vec3(0.0f, -0.1f, 0.0f);

    Pos_Vel_Rot(glm::vec3 _pos = glm::vec3(0.0f),
                glm::vec3 _vel = glm::vec3(0.0f),
                float _rot_angle = 0.0f,
                bool _anchored = true,
                bool _buoyant = false)
    {
        pos = _pos;
        vel = _vel;
        rot_angle = _rot_angle;
        anchored = _anchored;
        buoyant = _buoyant;
    }

    void update(float delta_time)
    {
        if (!anchored)
        {
            if (abs(vel.y) < 0.03f)
                vel.y += GRAVITY;

            if (abs(vel.x < 0.05f))
                vel.x += THRUST;

            vel *= FRICTION;
            pos += vel;
            rot_angle = glm::mix(rot_angle, -25.0f, delta_time * 0.2f);
        }
    }
};



class App
{
    public:
        App(int _screen_width, int _screen_height);

        bool create();
        void run();
        void update();
        void render();
        void render_text(const char* str,
                         glm::vec2 pos = glm::vec3(1.0f),
                         glm::vec3 color = glm::vec3(1.0f),
                         float scale = 1.0f,
                         bool aligned = true);
        bool gl_config();
        void load_shaders();
        void init_framebuffer();
        void load_models();
        void load_text();
        void test_for_torpedo_hit();
        void restart_game();
        void process_input();


    private:
        GLFWwindow* window;
        int screen_width;
        int screen_height;

        const float move_speed = 0.03;

        const float fog_scalar_min = 1.0f;
        const float fog_scalar_max = 1.0f;
        glm::vec3 fog_color = glm::vec3(0.0f, 0.01f, 0.1f);
        const float brightness = 1.8f;

        Shader text_shader;
        Shader shader;
        Shader bg_shader;
        Shader ice_shader;
        Shader particle_shader;
        Shader debug_shader;

        std::shared_ptr<Player> ship;
        std::shared_ptr<Particle_System> ship_bubbles_ps;
        std::shared_ptr<Object> torpedo;
        std::vector<Pos_Vel_Rot> torpedos;
        std::shared_ptr<Object> mine;
        std::vector<Pos_Vel_Rot> mines;
        std::shared_ptr<Object> background;
        std::shared_ptr<Object> ocean_surface;
        std::shared_ptr<Object> ocean_floor;
        std::shared_ptr<Object> base;
        std::vector<std::shared_ptr<Object>> icebergs;

        std::vector<glm::vec3> iceberg_start_positions;
        std::vector<glm::vec3> mine_start_positions;
        glm::vec3 ocean_floor_start_position;
        glm::vec3 base_start_position;

        Timer lose_timer = Timer(100);

        GLTtext *screen_text;
        glm::vec3 text_color = glm::vec3(1.0f, 1.0f, 1.0f);

        int player_score = 0;

        bool collision = false;

        unsigned int cubeVAO;
        unsigned int planeVAO;
        unsigned int quadVAO;

        unsigned int cube_texture;

        unsigned int framebuffer;
        unsigned int texture_colorbuffer;
        unsigned int rbo;

};


#endif
