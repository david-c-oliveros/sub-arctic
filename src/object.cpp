#include "object.h"



Object::Object()
{
}



Object::Object(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos, glm::vec3 _rot_angles, float _scale, bool _is_player, glm::vec3 _up)
    : model_mesh(_model_mesh), pos(_pos), rot_angles(_rot_angles), is_player(_is_player), scale(_scale)
{
    next_pos = pos;
    vel = glm::vec3(0.0f);
    world_up = _up;

    front = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
}



Object::Object(const char* model_path, glm::vec3 _pos, glm::vec3 _rot_angles, float _scale, bool _is_player, glm::vec3 _up)
    : pos(_pos), rot_angles(_rot_angles), is_player(_is_player), scale(_scale)
{
    model_mesh = std::make_shared<Model>(model_path);
    next_pos = pos;
    vel = glm::vec3(0.0f);
    world_up = _up;

    front = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
}



void Object::create(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos, float _rot_angle)
{
}



void Object::update()
{
    if (is_player)
        pos += vel;

}



void Object::draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot_angles.x), front);
    model = glm::rotate(model, glm::radians(rot_angles.y), up);
    model = glm::rotate(model, glm::radians(rot_angles.z), right);
    model = glm::scale(model , glm::vec3(scale, scale, scale));

    shader.set_mat4("model", model);
    model_mesh->draw(shader);
}
