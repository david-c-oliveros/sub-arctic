#include "object.h"



Object::Object()
{
}



Object::Object(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos,
               float _rot_angle, float _scale,
               glm::vec3 collider_dim, glm::vec3 _up)
    : model_mesh(_model_mesh), pos(_pos), rot_angle(_rot_angle), scale(_scale)
{
    next_pos = pos;
    vel = glm::vec3(0.0f);
    world_up = _up;

    front = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
    collider = std::make_shared<Box_Collider>(pos, collider_dim);
}



Object::Object(std::string model_path, glm::vec3 _pos,
               float _rot_angle, float _scale,
               glm::vec3 collider_dim, glm::vec3 _up)
    : pos(_pos), rot_angle(_rot_angle), scale(_scale)
{
    model_mesh = std::make_shared<Model>(model_path.c_str());
    next_pos = pos;
    vel = glm::vec3(0.0f);
    world_up = _up;

    front = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
    collider = std::make_shared<Box_Collider>(pos, collider_dim);
}



void Object::update(glm::vec3 new_vel)
{
    vel = new_vel;
    pos += vel;
    collider->update_pos(pos);
}



void Object::draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot_angle), right);
    model = glm::scale(model , glm::vec3(scale, scale, scale));

    shader.set_mat4("model", model);
    model_mesh->draw(shader);
}
