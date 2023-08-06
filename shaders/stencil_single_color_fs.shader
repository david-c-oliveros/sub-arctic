#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

float near =   0.1;
float far  = 10.0;

uniform sampler2D texture1;


void main()
{
    frag_color = vec4(0.04, 0.28, 0.26, 1.0);
}
