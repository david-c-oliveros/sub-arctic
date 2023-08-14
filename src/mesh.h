#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <glad.h>
//#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#define MAX_BONE_INFLUENCE 4


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;

    glm::vec3 tangent;
    glm::vec3 bitangent;

    int m_boneIDs[MAX_BONE_INFLUENCE];
    float m_weights[MAX_BONE_INFLUENCE];
};


struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};



class Mesh
{
    public:
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int vao;


    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void draw(Shader &shader);


    private:
        unsigned int vbo, ebo;
        
        void setup_mesh();
};


#endif
