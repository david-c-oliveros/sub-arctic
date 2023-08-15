#include "particle_system.h"



Particle_System::Particle_System(int _amount, float _avg_scale)
    : amount(_amount), avg_scale(_avg_scale)
{
    init();
}



void Particle_System::update(float delta_time, std::shared_ptr<Object> object, int new_particles, glm::vec2 offset)
{
    /***********************************/
    /*        Add New Particles        */
    /***********************************/
    for (int i = 0; i < new_particles; i++)
    {
        particles.push_back(Particle());
        int unused_particle = first_unused_particle();
        respawn_particle(particles[unused_particle], object, offset);
    }


    /**************************************/
    /*        Update All Particles        */
    /**************************************/
    for (auto &p : particles)
    {
        p.life -= delta_time;
        if (p.life > 0.0f)
        {
            p.pos -= p.vel * delta_time + glm::vec2(0.0f, -0.1f);
            p.color.x -= delta_time * 0.3f;
            p.color.y -= delta_time * 0.3f;
            p.color.z -= delta_time * 0.3f;
            p.color.a -= delta_time * 0.3f;
        }
    }
}



void Particle_System::draw(Shader &shader)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (Particle p : particles)
    {
        if (p.life > 0.0f)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(p.pos, 0.0f));
            model = glm::scale(model, glm::vec3(avg_scale));

            shader.set_mat4("model", model);
            shader.set_vec2("offset", p.pos);
            shader.set_vec4("color", p.color);

            p_geometry->draw(shader);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



void Particle_System::init()
{
    p_geometry = std::make_shared<Model>("res/effects/particle.obj");
    
    for (int i = 0; i < amount; i++)
    {
        particles.push_back(Particle());
    }
}



int last_used_particle = 0;

int Particle_System::first_unused_particle()
{
    for (int i = last_used_particle; i < amount; i++)
    {
        if (particles[i].life <= 0.0f)
        {
            last_used_particle = i;
            return i;
        }
    }

    for (int i = 0; i < last_used_particle; i++)
    {
        if (particles[i].life <= 0.0f)
        {
            last_used_particle = i;
            return i;
        }
    }

    last_used_particle = 0;
    return 0;
}



void Particle_System::respawn_particle(Particle &particle, std::shared_ptr<Object> object, glm::vec2 offset)
{
    //float random = ((std::rand() % 100) - 50) / 10.0f;
    glm::vec2 random = glm::diskRand(0.5f);
    //float r_color = 0.5f + ((std::rand() % 100) / 100.0f);
    float r_color = 1.0f;
    particle.pos = glm::vec2(object->pos.x, object->pos.y) + random + offset;
    particle.color = glm::vec4(r_color, r_color, r_color, 1.0f);
    particle.life = 3.0f;
    particle.vel = glm::vec2(object->vel.x, object->vel.y) * 0.1f;
}
