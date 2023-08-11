#ifndef APP_H
#define APP_H

#include <iostream>
#include <math.h>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void process_input(GLFWwindow* window, std::shared_ptr<Player> ship, float move_speed,
                   std::shared_ptr<Object> background, ma_engine audio_engine);

std::vector<glm::vec3> load_position_data(const char* filepath);


class App
{
    public:
        App(int _screen_width, int _screen_height);

        bool create();
        void run();
        void update();
        void render();
        bool gl_config();
        void load_shaders();
        void init_framebuffer();
        void load_models();


    private:
        GLFWwindow* window;
        int screen_width;
        int screen_height;

        float move_speed = 0.05;

        Shader shader;
        Shader bg_shader;
        Shader ice_shader;
        Shader debug_shader;

        std::shared_ptr<Player> ship;
        std::shared_ptr<Object> background;
        std::shared_ptr<Object> ocean_surface;
        std::shared_ptr<Object> ocean_floor;
        std::shared_ptr<Object> debug_cube;
        std::shared_ptr<Object> mine;
        std::shared_ptr<Object> base;
        std::vector<std::shared_ptr<Object>> icebergs;

        unsigned int cubeVAO;
        unsigned int planeVAO;
        unsigned int quadVAO;

        unsigned int cube_texture;

        unsigned int framebuffer;
        unsigned int texture_colorbuffer;
        unsigned int rbo;

};


#endif
