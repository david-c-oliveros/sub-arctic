#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <memory>

#include <glm/glm.hpp>

#include "shader.h"



class Box_Collider
{
    public:
        Box_Collider(glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 size   = glm::vec3(2.0f, 2.0f, 2.0f));
        ~Box_Collider();

        void update_pos(glm::vec3 new_pos);
        glm::vec3 get_pos();
        void draw(Shader &shader);


        glm::vec3 dim;
        glm::vec3 origin;
        glm::vec3 min;
        glm::vec3 max;
        glm::vec3 vel;
        glm::vec3 scale;
        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);


    private:
        void setup_mesh();


        unsigned int vbo, vao, ebo;
        static constexpr float mesh_vertices[] =
        {   
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,  
            -0.5f,  0.5f, 0.0f,
        };


        static constexpr unsigned int mesh_indices[] = {
            0, 1, 3,
            1, 2, 3
        };


    public:
        static glm::vec3 resolve_collisions(std::shared_ptr<Box_Collider> a, glm::vec3 a_vel, std::shared_ptr<Box_Collider> b);
        static bool aabb_collide(std::shared_ptr<Box_Collider> a, std::shared_ptr<Box_Collider> b);


    private:
        static glm::vec3 calc_aabb_distance_to(std::shared_ptr<Box_Collider> a, std::shared_ptr<Box_Collider> b);
};


#endif
