#include "object.h"



Object::Object()
{
}



Object::Object(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos,float _rot_angle, float _scale, bool _is_player, glm::vec3 _up)
    : model_mesh(_model_mesh), pos(_pos), rot_angle(_rot_angle), is_player(_is_player), scale(_scale)
{
    next_pos = pos;
    vel = glm::vec3(0.0f);
    world_up = _up;

    front = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
}



Object::Object(const char* model_path, glm::vec3 _pos, float _rot_angle, float _scale, bool _is_player, glm::vec3 _up)
    : pos(_pos), rot_angle(_rot_angle), is_player(_is_player), scale(_scale)
{
    model_mesh = std::make_shared<Model>(model_path);
    next_pos = pos;
    vel = glm::vec3(0.0f);
    world_up = _up;

    front = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
}



void Object::update(float value, float delta_time)
{
    pos.x -= value * delta_time;
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
