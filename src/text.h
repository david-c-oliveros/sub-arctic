#ifndef TEXT_H
#define TEXT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"



struct Character
{
    unsigned int texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};



class Text
{
    public:
        unsigned int vao, vbo;
        std::map<char, Character> characters;
        glm::mat4 projection;


    public:
        Text(int scr_width, int scr_height);
        ~Text();

        void render_text(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);


    private:
        bool initialize();
        void setup_buffers();
};


#endif
