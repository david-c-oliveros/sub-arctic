#include "camera.h"



Camera::Camera(glm::vec3 _position, glm::vec3 _up, float _yaw, float _pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
{
    position = _position;
    world_up = _up;
    yaw      = _yaw;
    pitch    = _pitch;
    update_camera_vectors();
}



Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float _yaw, float _pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED), mouse_sensitivity(SENSITIVITY), zoom(ZOOM)
{
    position = glm::vec3(pos_x, pos_y, pos_z);
    world_up = glm::vec3(up_x, up_y, up_z);
    yaw      = _yaw;
    pitch    = _pitch;
    update_camera_vectors();
}



glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position, position + front, up);
}



void Camera::process_keyboard(Camera_Movement direction, float delta_time)
{
    float velocity = movement_speed * delta_time;
    if (direction == UP)
        position += world_up * velocity;
    if (direction == DOWN)
        position -= world_up * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}



void Camera::process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (constrain_pitch)
    {
        if (pitch >  89.0f)
            pitch =  89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    update_camera_vectors();
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
