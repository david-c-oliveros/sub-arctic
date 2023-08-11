#include "camera.h"



Camera::Camera(glm::vec3 _pos, glm::vec3 _up, float _yaw, float _pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
{
    pos = _pos;
    world_up = _up;
    yaw      = _yaw;
    pitch    = _pitch;
    debug = false;
    debug_pos = pos;
    debug_yaw = yaw;
    debug_pitch = pitch;
    update_camera_vectors();
    update_debug_vectors();
}



Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float _yaw, float _pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
{
    pos = glm::vec3(pos_x, pos_y, pos_z);
    world_up = glm::vec3(up_x, up_y, up_z);
    yaw      = _yaw;
    pitch    = _pitch;
    debug = false;
    debug_pos = pos;
    debug_yaw = yaw;
    debug_pitch = pitch;
    update_camera_vectors();
    update_debug_vectors();
}



glm::mat4 Camera::get_view_matrix()
{
    if (debug)
        return glm::lookAt(debug_pos, debug_pos + debug_front, debug_up);

    return glm::lookAt(pos, pos + front, up);
}



void Camera::process_keyboard(Camera_Movement direction, float delta_time)
{
    float velocity = 100.0f * delta_time;
    if (direction == UP)
        debug_pos += debug_front * velocity;
    if (direction == DOWN)
        debug_pos -= debug_front * velocity;
    if (direction == LEFT)
        debug_pos -= debug_right * velocity;
    if (direction == RIGHT)
        debug_pos += debug_right * velocity;
}



void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    debug_yaw   += xoffset;
    debug_pitch += yoffset;

    if (constrain_pitch)
    {
        if (debug_pitch >  89.0f)
            debug_pitch =  89.0f;
        if (debug_pitch < -89.0f)
            debug_pitch = -89.0f;
    }

    update_camera_vectors();
    update_debug_vectors();
}



void Camera::process_mouse_scroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom <  1.0f)
        zoom =  1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}



void Camera::update_camera_vectors()
{
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);

    right = glm::normalize(glm::cross(front, world_up));
    up    = glm::normalize(glm::cross(right, front));
}




void Camera::update_debug_vectors()
{
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(debug_yaw)) * cos(glm::radians(debug_pitch));
    new_front.y = sin(glm::radians(debug_pitch));
    new_front.z = sin(glm::radians(debug_yaw)) * cos(glm::radians(debug_pitch));
    debug_front = glm::normalize(new_front);

    debug_right = glm::normalize(glm::cross(debug_front, world_up));
    debug_up    = glm::normalize(glm::cross(debug_right, debug_front));
}
