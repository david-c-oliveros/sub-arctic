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
        glm::vec3 vel;

        /**************************/
        /*        Yaw:   y        */
        /*        Pitch: z        */
        /*        Roll:  x        */
        /**************************/
        glm::vec3 rot_angles;

        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 world_up;

        float yaw;
        float pitch;

        bool is_player;
        float scale;


    public:
        Object();
        Object(std::shared_ptr<Model> _model_mesh,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 _rot_angles = glm::vec3(0.0f, 0.0f, 0.0f),
                float _scale = 1.0f, bool _is_player = false,
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        Object(const char* model_path,
                glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3 _rot_angles = glm::vec3(0.0f, 0.0f, 0.0f),
                float _scale = 1.0f, bool _is_player = false,
                glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f));

        void create(std::shared_ptr<Model> _model_mesh, glm::vec3 _pos, float _rot_angle);
        void update();
        void draw(Shader &shader);
};


#endif
