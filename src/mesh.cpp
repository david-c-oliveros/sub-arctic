#include "mesh.h"



Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures)
{
    vertices = _vertices;
    indices  = _indices;
    textures = _textures;

    setup_mesh();
}



void Mesh::draw(Shader &shader)
{
    // Bind appropriate textures
    unsigned int diffuse_nr  = 1;
    unsigned int specular_nr = 1;
    unsigned int normal_nr   = 1;
    unsigned int height_nr   = 1;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding

        // Retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuse_nr++);  // Transfer unsigned int to string
        else if (name == "texture_specular")
            number = std::to_string(specular_nr++); // Transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normal_nr++);   // Transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(height_nr++);   // Transfer unsigned int to string

        // Now set the sampler to the correct texture unit
        //glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
        //shader.set_float(("material." + name + number).c_str(), i);
        shader.set_float((name + number).c_str(), i);
        //std::cout << name << " " << textures[i].path << std::endl;
        //std::cout << name << " " << number << std::endl;

        // And finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    /***************************/
    /*        Draw Mesh        */
    /***************************/

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}



void Mesh::setup_mesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    /**********************************/
    /*        Vertex Positions        */
    /**********************************/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    /********************************/
    /*        Vertex Normals        */
    /********************************/
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    /***************************************/
    /*        Vertex Texture Coords        */
    /***************************************/
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    /********************************/
    /*        Vertex Tangent        */
    /********************************/
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    /**********************************/
    /*        Vertex Bitangent        */
    /**********************************/
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    /*********************/
    /*        IDs        */
    /*********************/
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_boneIDs));

    /*************************/
    /*        Weights        */
    /*************************/
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));

    glBindVertexArray(0);
}
