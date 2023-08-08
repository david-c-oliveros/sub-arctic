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
                glm::vec3 _rot_angles = glm::vec3(0.0f, 0.0f, 0.0f),
                float _scale = 1.0f, bool _is_player = false,
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        Player(const char* model_path,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 _rot_angles = glm::vec3(0.0f, 0.0f, 0.0f),
                float _scale = 1.0f, bool _is_player = false,
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        ~Player();

        void update(float delta_time);
        void apply_drag();


    public:
        Movement input_dir;


    private:
        glm::vec3 neutral_pos;
};


#endif
