#version 330 core

out vec4 frag_color;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform vec3 view_pos;
uniform vec3 fog_color;
uniform float brightness;

in vec3 frag_pos;
in vec3 frag_world;


#define NUM_OCTAVES 5



vec3 generate_noise();
float random(vec2 _st);
float fbm(vec2 _st);
float fog_factor(float d);



void main()
{
    vec3 result = generate_noise();

    result *= vec3(8.0, 8.0, 8.0);

    vec4 camera_eye = vec4(view_pos, 1.0);
    float d = distance(camera_eye, vec4(frag_pos, 1.0));
    float alpha = fog_factor(d);

    result = mix(result, fog_color, alpha);

    result *= brightness;

    frag_color = vec4(result, 1.0);
}



bool isnan( float val )
{
  return ( val < 0.0 || 0.0 < val || val == 0.0 ) ? false : true;
}

float random(vec2 _st)
{
    return fract(sin(dot(_st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}



float noise(in vec2 _st)
{
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
        (c - a) * u.y * (1.0 - u.x) +
        (d - b) * u.x * u.y;
}



float fbm (vec2 _st)
{
    float value = 0.1;
    float amplitude = 0.3;
    vec2 shift = vec2(100.0);

    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));

    for (int i = 0; i < NUM_OCTAVES; ++i)
    {
        value += amplitude * noise(_st);
        _st = rot * _st * 3.2 + shift;
        amplitude *= 0.5;
    }

    return value;
}


vec3 generate_noise()
{
    vec2 st = vec2(frag_world.x/1.4, frag_world.y)/u_resolution.xy*20;

    vec3 color = vec3(0.0, 0.0, 0.01);

    vec2 q = vec2(0.);
    q.x = fbm(st + 0.00*u_time);
    q.y = fbm(st + vec2(1.0));

    vec2 r = vec2(0.);
    r.x = fbm(st + 1.0*q + vec2(1.7, 9.2) + 0.037*u_time);
    r.y = fbm(st + 1.0*q + vec2(2.3, 2.8) + 0.031*u_time);

    float f = fbm(st + r);

    color = mix(vec3(0.101961, 0.619608, 0.666667),
                vec3(0.666667, 0.666667, 0.498039),
                clamp((f*f)*4.0, 0.0, 1.0));

    color = mix(color,
                vec3(0,0,0.164706),
                clamp(length(q), 0.0, 1.0));

    color = mix(color,
                vec3(0.666667, 1, 1),
                clamp(length(r.x), 0.0, 1.0));

    color *= vec3(1.0, 1.3, 1.5);

    return vec3(f*f*f + .004*f*f + .3*f) * color;
}



float fog_factor(float d)
{
    const float fog_max = 130.0;
    const float fog_min = 0.0;

    if (d >= fog_max)
        return 1.;

    if (d <= fog_min)
        return 0.;

    return 1 - (fog_max - d) / (fog_max - fog_min);
}
