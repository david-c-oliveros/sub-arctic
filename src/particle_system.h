#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "model.h"
#include "object.h"

#include "particle.h"



class Particle_System
{
    public:
        Particle_System(int _amount, float _avg_scale = 1.0f);
        void update(float delta_time, std::shared_ptr<Object> object, int new_particles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
        void draw(Shader &shader);


    private:
        std::vector<Particle> particles;
        unsigned int amount;

        float avg_scale;
        std::shared_ptr<Model> p_geometry;


    private:
        void init();
        int first_unused_particle();
        void respawn_particle(Particle &particle, std::shared_ptr<Object> object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif
