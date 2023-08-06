#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "model.h"



class Object
{
    private:
        std::shared_ptr<Model> model_mesh;

    public:
        glm::vec3 pos;
        glm::vec3 next_pos;
        glm::vec3 velocity;
        float rot_angle;


    public:
        Object();
        Object(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), float _rot_angle = 0.0f);
        Object(const char* model_path, glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), float _rot_angle = 0.0f);

        void create(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos, float _rot_angle);
        void draw(Shader &shader);
};


#endif
