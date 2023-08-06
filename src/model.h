#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int texture_from_file(const char* path, const std::string &directory, bool gamma = false);



class Model
{
    public:
        // Model Data
        std::vector<Texture> textures_loaded;   // Stores all textures loaded so far, optimization to make sure textures are not loaded more than once
        std::vector<Mesh>    meshes;
        std::string directory;
        bool gamma_correction;


    public:
        Model(const std::string &path, bool gamma = false);

        void draw(Shader &shader);


    private:
        void load_model(const std::string &path);
        void process_node(aiNode* node, const aiScene* scene);
        Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);
};


#endif
