#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <glm/glm.hpp>



class Particle
{
    public:
        Particle();


    public:
        glm::vec2 pos;
        glm::vec2 vel;
        glm::vec4 color;
        float     life;
};


#endif
