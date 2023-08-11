#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 frag_world;
out vec3 frag_local;
out vec3 normal;
out vec2 tex_coords;

uniform mat4 model;
uniform mat4 model_origin;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    frag_world = vec3(model * vec4(aPos, 1.0));
    frag_local = aPos;
    normal = mat3(transpose(inverse(model))) * aNormal;

    tex_coords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
