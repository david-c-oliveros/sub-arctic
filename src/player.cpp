#include "player.h"



Player::Player(std::shared_ptr<Model> _model_mesh,
        glm::vec3 _pos,
        float _rot_angle,
        float _scale, bool _is_player,
        glm::vec3 _up)
    : Object(_model_mesh, _pos, _rot_angle, _scale, _is_player, _up)
{
    input_dir = Movement::NONE;
    neutral_rot = 0.0f;
}

Player::Player(const char* model_path,
        glm::vec3 _pos,
        float _rot_angle,
        float _scale, bool _is_player,
        glm::vec3 _up)
    : Object(model_path, _pos, _rot_angle, _scale, _is_player, _up)
{
    input_dir = Movement::NONE;
    neutral_rot = 0.0f;
}



Player::~Player()
{
}



void Player::update(float delta_time)
{
    apply_drag();
    pos += vel;

    switch (input_dir)
    {
        case Movement::NONE:
            rot_angle = glm::mix(rot_angle, neutral_rot, delta_time * 0.5f);
            break;

        case Movement::UP:
            rot_angle = glm::mix(rot_angle, max_rot, delta_time * 0.2f);
            break;

        case Movement::DOWN:
            rot_angle = glm::mix(rot_angle, -max_rot, delta_time * 0.2f);
            break;
    }
}



void Player::apply_drag()
{
    vel += -vel * 0.01f;
}
