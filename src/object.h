#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "model.h"
#include "box_collider.h"



class Object
{
    private:
        std::shared_ptr<Model> model_mesh;

    public:
        std::shared_ptr<Box_Collider> collider;
        glm::vec3 pos;
        glm::vec3 next_pos;
        glm::vec3 vel;

        /**************************/
        /*        Yaw:   y        */
        /*        Pitch: z        */
        /*        Roll:  x        */
        /**************************/
        float rot_angle;

        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        bool is_player;
        float scale;


    public:
        Object();
        Object(std::shared_ptr<Model> _model_mesh,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                float _rot_angles = 0.0f,
                float _scale = 1.0f,
                glm::vec3 collider_dim = glm::vec3(2.0f),
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        Object(const char* model_path,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                float _rot_angles = 0.0f,
                float _scale = 1.0f,
                glm::vec3 collider_dim = glm::vec3(2.0f),
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        void update(glm::vec3 new_vel);
        void draw(Shader &shader);
};


#endif
