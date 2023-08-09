#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coords;
out vec4 vertex_world;
out vec4 vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    frag_pos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;

    vertex = vec4(aPos, 1.0);

    tex_coords = aTexCoords;
    mat4 mv = view * model;
    vertex_world = mv * vec4(aPos, 1.0);
    vertex_world = inverse(mv) * vertex_world;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
