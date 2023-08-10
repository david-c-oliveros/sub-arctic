#include "box_collider.h"



Box_Collider::Box_Collider(glm::vec3 origin, glm::vec3 size)
{
    dim.x = size.x / 2;
    dim.y = size.y / 2;
    dim.z = size.z / 2;

    min = origin - dim;
    max = origin + dim;
}



Box_Collider::~Box_Collider()
{
}



void Box_Collider::update_pos(glm::vec3 new_pos)
{
    origin = new_pos;
    min = origin - dim;
    max = origin + dim;
    vel = glm::vec3(0.0f);
}



glm::vec3 Box_Collider::get_pos()
{
    return origin;
}



glm::vec3 Box_Collider::resolve_collisions(std::shared_ptr<Box_Collider> a, glm::vec3 a_vel, std::shared_ptr<Box_Collider> b)
{
    glm::vec3 distance = calc_aabb_distance_to(a, b);
    glm::vec3 move_delta = glm::vec3(0.0f);
    glm::vec3 time_to_collide = glm::vec3(0.0f);
    time_to_collide.x = a_vel.x != 0.0f ? abs(distance.x / a_vel.x) : 0.0f;
    time_to_collide.y = a_vel.y != 0.0f ? abs(distance.z / a_vel.y) : 0.0f;

    float shortest_time = 0.0f;
    
    if (a_vel.x != 0.0f && a_vel.y == 0.0f)
    {
        shortest_time = time_to_collide.x;
        move_delta.x = shortest_time * a_vel.x;
    }
    else if (a_vel.x == 0.0f && a_vel.y != 0.0f)
    {
        shortest_time = time_to_collide.x;
        move_delta.x = shortest_time * a_vel.x;
    }
    else
    {
        shortest_time = std::min(abs(time_to_collide.x), abs(time_to_collide.y));
        move_delta.x = shortest_time * a_vel.x;
        move_delta.y = shortest_time * a_vel.y;
    }

    return move_delta;
}



glm::vec3 Box_Collider::calc_aabb_distance_to(std::shared_ptr<Box_Collider> a, std::shared_ptr<Box_Collider> b)
{
    glm::vec3 delta = glm::vec3(0.0f);

    /************************/
    /*        X Axis        */
    /************************/
    if (a->min.x < b->min.x)
    {
        delta.x = b->min.x - a->max.x;
    }
    else if (a->min.x > b->min.x)
    {
        delta.x = a->min.x - b->max.x;
    }

    /************************/
    /*        X Axis        */
    /************************/
    if (a->min.z < b->min.z)
    {
        delta.z = b->min.z - a->max.z;
    }
    else if (a->min.z > b->min.z)
    {
        delta.z = a->min.z - b->max.z;
    }

    return delta;
}



bool Box_Collider::aabb_collide(std::shared_ptr<Box_Collider> a, std::shared_ptr<Box_Collider> b)
{
    return (a->min.x < b->max.x && a->max.x > b->min.x) &&
           (a->min.y < b->max.y && a->max.y > b->min.y) &&
           (a->min.z < b->max.z && a->max.z > b->min.z);
}

