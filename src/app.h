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

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "object.h"
#include "player.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window, std::shared_ptr<Player> ship, float move_speed, std::shared_ptr<Object> background);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

        float move_speed = 0.2;

        Shader shader;

        std::shared_ptr<Player> ship;
        std::shared_ptr<Object> backpack;
        std::shared_ptr<Object> background;

        unsigned int cubeVAO;
        unsigned int planeVAO;
        unsigned int quadVAO;

        unsigned int cube_texture;

        unsigned int framebuffer;
        unsigned int texture_colorbuffer;
        unsigned int rbo;
};


#endif
