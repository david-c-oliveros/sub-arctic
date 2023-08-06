#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

float near =   0.1;
float far  = 10.0;

uniform sampler2D texture1;


void main()
{
    frag_color = texture(texture1, tex_coords);
}
