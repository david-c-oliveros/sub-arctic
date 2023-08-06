#include "object.h"



Object::Object()
{
}



Object::Object(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos, float _rot_angle)
    : model_mesh(_model_mesh), pos(_pos), rot_angle(_rot_angle)
{
    next_pos = pos;
    velocity = glm::vec3(0.0f);
}



Object::Object(const char* model_path, glm::vec3 _pos, float _rot_angle)
    : pos(_pos), rot_angle(_rot_angle)
{
    model_mesh = std::make_shared<Model>(model_path);
    next_pos = pos;
    velocity = glm::vec3(0.0f);
}



void Object::create(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos, float _rot_angle)
{
}



void Object::draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rot_angle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, pos);
    shader.set_mat4("model", model);
    model_mesh->draw(shader);
}
