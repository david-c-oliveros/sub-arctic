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

#include "stb_image.h"

#include "shader.h"
#include "camera.h"
#include "model.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos_in, double ypos_in);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow* window);

class App
{
    public:
        GLFWwindow* window;
        int screen_width;
        int screen_height;

        std::shared_ptr<Shader> shader;
        std::shared_ptr<Shader> screen_shader;

        unsigned int cubeVAO;
        unsigned int planeVAO;
        unsigned int quadVAO;

        unsigned int cube_texture;

        unsigned int framebuffer;
        unsigned int texture_colorbuffer;
        unsigned int rbo;

        // Timing


    public:
        App(int _screen_width, int _screen_height);

        bool create();
        void run();
        bool gl_config();
        void load_shaders();
        void init_framebuffer();
        void load_models();
};


#endif
