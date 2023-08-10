#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <glm/gtx/string_cast.hpp>

#include "object.h"



enum class Movement
{
    NONE,
    UP,
    DOWN
};



class Player : public Object
{
    public:
        Player();
        Player(std::shared_ptr<Model> _model_mesh,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                float _rot_angle = 0.0f,
                float _scale = 1.0f,
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3 collider_dim = glm::vec3(2.0f));

        Player(const char* model_path,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                float _rot_angle = 0.0f,
                float _scale = 1.0f,
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3 collider_dim = glm::vec3(2.0f));

        ~Player();

        void update(float delta_time);
        void apply_drag();


    public:
        Movement input_dir;


    private:
        float neutral_rot;
        float max_rot = 20.0f;
};


#endif
