#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include <memory>

#include <glm/glm.hpp>



class Box_Collider
{
    public:
        Box_Collider(glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3 size   = glm::vec3(2.0f, 2.0f, 2.0f));
        ~Box_Collider();

        void update_pos(glm::vec3 new_pos);
        glm::vec3 get_pos();


    public:
        static glm::vec3 resolve_collisions(std::shared_ptr<Box_Collider> a, glm::vec3 a_vel, std::shared_ptr<Box_Collider> b);
        static bool aabb_collide(std::shared_ptr<Box_Collider> a, std::shared_ptr<Box_Collider> b);


    public:
        glm::vec3 dim;
        glm::vec3 origin;
        glm::vec3 min;
        glm::vec3 max;
        glm::vec3 vel;


    private:
        static glm::vec3 calc_aabb_distance_to(std::shared_ptr<Box_Collider> a, std::shared_ptr<Box_Collider> b);
};


#endif
