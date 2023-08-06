#version 330 core

out vec4 frag_color;

in vec2 tex_coords;

uniform sampler2D screen_texture;

const float offset = 1.0 / 300;

void main()
{
    // Greyscale
//    frag_color = texture(screen_texture, tex_coords);
//    float average = 0.2126 * frag_color.r + 0.7152 * frag_color.g + 0.0722 * frag_color.b;
//    frag_color = vec4(average, average, average, 1.0);

//    // Inverted Color
//    frag_color = vec4(vec3(1.0 - texture(screen_texture, tex_coords)), 1.0);

    // Kernel
//    vec2 offsets[9] = vec2[](
//        vec2(-offset,  offset), // top-left
//        vec2( 0.0f,    offset), // top-center
//        vec2( offset,  offset), // top-right
//        vec2(-offset,  0.0f),   // center-left
//        vec2( 0.0f,    0.0f),   // center-center
//        vec2( offset,  0.0f),   // center-right
//        vec2(-offset, -offset), // bottom-left
//        vec2( 0.0f,   -offset), // bottom-center
//        vec2( offset, -offset)  // bottom-right    
//    );

//    float kernel[9] = float[](
//            -1, -1, -1,
//            -1,  9, -1,
//            -1, -1, -1
//        );

//    float kernel[9] = float[](
//            1.0 / 16, 2.0 / 16, 1.0 / 16,
//            2.0 / 16, 4.0 / 16, 2.0 / 16,
//            1.0 / 16, 2.0 / 16, 1.0 / 16
//            );

//    float kernel[9] = float[](
//            1,  1, 1,
//            1, -8, 1,
//            1,  1, 1
//            );
//
//    vec3 sample_tex[9];
//    for (int i = 0; i < 9; i++)
//    {
//        sample_tex[i] = vec3(texture(screen_texture, tex_coords.st + offsets[i]));
//    }
//
//    vec3 col = vec3(0.0);
//    for (int i = 0; i < 9; i++)
//        col += sample_tex[i] * kernel[i];

//    frag_color = vec4(col, 1.0);
    frag_color = texture(screen_texture, tex_coords);
}
