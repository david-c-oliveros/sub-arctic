#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D texture1;
uniform sampler2D texture2;

const float offset = 1.0 / 300;

void main()
{
    frag_color = texture(texture2, tex_coords);
}
