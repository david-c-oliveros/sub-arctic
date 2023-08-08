#include "player.h"



Player::Player(std::shared_ptr<Model> _model_mesh,
        glm::vec3 _pos,
        glm::vec3 _rot_angles,
        float _scale, bool _is_player,
        glm::vec3 _up)
    : Object(_model_mesh, _pos, _rot_angles, _scale, _is_player, _up)
{
    input_dir = Movement::NONE;
    neutral_pos = glm::vec3(_pos);
}

Player::Player(const char* model_path,
        glm::vec3 _pos,
        glm::vec3 _rot_angles,
        float _scale, bool _is_player,
        glm::vec3 _up)
    : Object(model_path, _pos, _rot_angles, _scale, _is_player, _up)
{
    input_dir = Movement::NONE;
    neutral_pos = glm::vec3(_pos);
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
            rot_angles.z = glm::mix(rot_angles.z, neutral_pos.z, delta_time);
            break;

        case Movement::UP:
            rot_angles.z = glm::mix(rot_angles.z, 10.0f, delta_time);
            break;

        case Movement::DOWN:
            rot_angles.z = glm::mix(rot_angles.z, -10.0f, delta_time);
            break;
    }
}



void Player::apply_drag()
{
    vel += -vel * 0.01f;
}
