#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>
//#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>


enum Camera_Movement
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   2.5f;
const float SENSITIVITY =   0.05f;
const float ZOOM        =  45.0f;



class Camera
{
    public:
        glm::vec3 pos;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        float yaw;
        float pitch;

        float movement_speed;
        float mouse_sensitivity;
        float zoom;
        bool debug;

        glm::vec3 debug_pos;
        glm::vec3 debug_front;
        glm::vec3 debug_right;
        glm::vec3 debug_up;


    public:
        Camera(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f),
               float _yaw = YAW, float _pitch = PITCH);

        Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float _yaw, float _pitch);

        glm::mat4 get_view_matrix();
        void process_keyboard(Camera_Movement direction, float delta_time);
        void process_mouse_movement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
        void process_mouse_scroll(float yoffset);


    private:
        void update_camera_vectors();
        void update_debug_vectors();


    private:

        float debug_yaw;
        float debug_pitch;
};


#endif
