#include "player.h"



Player::Player(std::shared_ptr<Model> _model_mesh,
        glm::vec3 _pos,
        glm::vec3 _rot_angles,
        float _scale, bool _is_player,
        glm::vec3 _up)
    : Object(_model_mesh, _pos, _rot_angles, _scale, _is_player, _up)
{
    input_dir = Movement::NONE;
}

Player::Player(const char* model_path,
        glm::vec3 _pos,
        glm::vec3 _rot_angles,
        float _scale, bool _is_player,
        glm::vec3 _up)
    : Object(model_path, _pos, _rot_angles, _scale, _is_player, _up)
{
    input_dir = Movement::NONE;
}



Player::~Player()
{
}



void Player::update()
{
    pos += vel;

    switch (input_dir)
    {
        case Movement::NONE:
            rot_angles.x = 0.0f;
            break;

        case Movement::UP:
            rot_angles.x = 30.0f;
            break;

        case Movement::DOWN:
            rot_angles.x = -30.0f;
            break;
    }
}
