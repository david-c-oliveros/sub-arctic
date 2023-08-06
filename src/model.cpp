#include "model.h"


Model::Model(const std::string &path, bool gamma)
            : gamma_correction(gamma)
{
    load_model(path);
}



void Model::draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}



void Model::load_model(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // If is not zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}



void Model::process_node(aiNode* node, const aiScene* scene)
{
    /***********************************************************/
    /*        Process all of the node's meshes (if any)        */
    /***********************************************************/
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    /***********************************************************/
    /*        Then do the same for each of its children        */
    /***********************************************************/
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}



Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    /**************************/
    /*        Vertices        */
    /**************************/
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vec;  // Placeholder vector

        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.position = vec;

        if (mesh->HasNormals())
        {
            vec.x = mesh->mNormals[i].x;
            vec.y = mesh->mNormals[i].y;
            vec.z = mesh->mNormals[i].z;
            vertex.normal = vec;
        }

        /*************************************/
        /*        Texture Coordinates        */
        /*************************************/
        if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 v;

            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we alwasy take the first set (0)
            v.x = mesh->mTextureCoords[0][i].x;
            v.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = v;

//            // Tangent
//            vec.x = mesh->mTangents[i].x;
//            vec.y = mesh->mTangents[i].y;
//            vec.z = mesh->mTangents[i].z;
//            vertex.tangent = vec;
//
//            // Bitangent
//            vec.x = mesh->mBitangents[i].x;
//            vec.y = mesh->mBitangents[i].y;
//            vec.z = mesh->mBitangents[i].z;
//            vertex.bitangent = vec;
        }
        else
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    /*************************/
    /*        Indices        */
    /*************************/
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    /***************************/
    /*        Materials        */
    /***************************/
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // We assume a convention for the sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequenctial number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse:  texture_diffuseN
        // specular: texture_specularN
        // normal:   texture_normalN

        // 1. Diffuse Maps
        std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        // 2. Specular Maps
        std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

//        // 3. Normal Maps
//        std::vector<Texture> normal_maps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
//        textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
//    
//        // 4. Height Maps
//        std::vector<Texture> height_maps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
//        textures.insert(textures.end(), height_maps.begin(), height_maps.end());
    }

    return Mesh(vertices, indices, textures);
}



std::vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }

        if (!skip)
        {
            // If texture hasn't been loaded already, load it
            Texture texture;
            texture.id = texture_from_file(str.C_Str(), directory);
            texture.type = type_name;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // Store it as texture loaded for entire model, to ensure we won't unnecessarily load duplicate textures
        }
    }

    return textures;
}



unsigned int texture_from_file(const char* path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nr_components;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nr_components, 0);

    if (data)
    {
        GLenum format;
        if (nr_components == 1)
            format = GL_RED;
        else if (nr_components == 3)
            format = GL_RGB;
        else if (nr_components == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return texture_id;
}
