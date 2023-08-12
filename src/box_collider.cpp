#include "box_collider.h"



Box_Collider::Box_Collider(glm::vec3 origin, glm::vec3 size)
{
    scale = size;
    dim.x = size.x / 2;
    dim.y = size.y / 2;
    dim.z = size.z / 2;

    min = origin - dim;
    max = origin + dim;

    setup_mesh();
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
           (a->min.y < b->max.y && a->max.y > b->min.y);
//    return (a->min.x < b->max.x && a->max.x > b->min.x) &&
//           (a->min.y < b->max.y && a->max.y > b->min.y) &&
//           (a->min.z < b->max.z && a->max.z > b->min.z);
}


void Box_Collider::setup_mesh()
{
    // Vertex Buffer
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_vertices), mesh_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh_indices), mesh_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}


void Box_Collider::draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, origin);
    model = glm::scale(model, scale);
    shader.set_mat4("model", model);

    shader.set_vec3("color", color);

    glBindVertexArray(vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
}
