#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

float near =   0.1;
float far  = 10.0;

uniform sampler2D texture1;


float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
    float depth = linearize_depth(gl_FragCoord.z) / far;
    //frag_color = vec4(vec3(depth), 1.0);
    frag_color = texture(texture1, tex_coords);
}
