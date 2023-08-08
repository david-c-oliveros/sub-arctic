#version 330 core

out vec4 frag_color;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

in vec3 frag_pos;


float random(vec2 _st)
{
    return fract(sin(dot(_st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}


float noise(in vec2 _st)
{
    vec2 i = floor(_st);
    vec2 f = floor(_st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}


#define NUM_OCTAVES 6


float fbm (vec2 _st)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 0;

    for (int i = 0; i < NUM_OCTAVES; ++i)
    {
        value += amplitude * noise(_st);
        _st *= 2.0;
        amplitude *= 0.5;
    }

    return value;
}


void main()
{
    vec2 st = frag_pos.xy/u_resolution.xy;
    //vec2 st = frag_pos.xy;
    st.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.0);
    color += fbm(st*3.0);

    frag_color = vec4(color, 1.0);
}
