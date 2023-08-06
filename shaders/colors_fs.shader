#version 330 core

out vec4 frag_color;

uniform vec3 object_color;
uniform vec3 light_color;

void main()
{
    // Linearly interpolate between both textures (80% texture1, 20% texture2)
    frag_color = vec4(light_color * object_color, 1.0);
}
