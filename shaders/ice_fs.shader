#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


out vec4 frag_color;


struct Dir_Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec2 u_resolution;
uniform float u_time;
uniform Dir_Light dir_light;
uniform Material material;
uniform vec3 view_pos;
uniform vec3 fog_color;

in vec3 frag_pos;
in vec3 frag_world;
in vec3 normal;
in vec2 tex_coords;
in vec4 vertex;
in vec4 vertex_world;


#define NUM_OCTAVES 5


vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir, vec3 noise);
vec3 generate_noise();
vec3 generate_noise3();
float fbm (vec2 _st);
float random(vec2 _st);
float fog_factor(float d);


void main()
{
    // Properties
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    vec3 noise = generate_noise();
    vec3 result = calc_dir_light(dir_light, norm, view_dir, noise);
    //result += vec3(0.0, 0.12, 0.15);
    //result += vec3(0.7, 0.7, 0.3) * generate_noise();
    //result *= generate_noise();

    vec4 camera_eye = vec4(view_pos, 1.0);
    float d = distance(camera_eye, vertex);
    float alpha = fog_factor(d);

    result = mix(result, fog_color, alpha);

    frag_color = vec4(result, 1.0);
}



vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir, vec3 noise)
{
    vec3 light_dir = normalize(-light.direction);

    // Diffuse Shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular Shading
    float shininess = 32.0;
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    // Combine Results
    vec3 ambient  = light.ambient  *        noise;
    vec3 diffuse  = light.diffuse  * diff * noise;
    vec3 specular = light.specular * spec * noise;

    return ambient + diffuse + specular;
}



float random(vec2 _st)
{
    return fract(sin(dot(_st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}



float noise(vec2 _st)
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
    float value = 0.0;
    float amplitude = 0.6;
    vec2 shift = vec2(100.0);

    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));

    for (int i = 0; i < NUM_OCTAVES; ++i)
    {
        value += amplitude * noise(_st);
        _st = rot * _st * 2.0 + shift;
        amplitude *= 0.5;
    }

    return value;
}


vec3 generate_noise()
{
    //vec2 st = gl_FragCoord.xy/u_resolution.xy*3;
    vec2 st = frag_world.xy;

    vec3 color = vec3(0.0);

    vec2 q = vec2(0.);
    q.x = fbm(st + 0.00*u_time);
    q.y = fbm(st + vec2(1.0));

    vec2 r = vec2(0.);
    r.x = fbm(st + 1.0*q + vec2(1.7, 9.2));// + 0.15*u_time);
    r.y = fbm(st + 1.0*q + vec2(8.3, 2.8));// + 0.126*u_time);

    float f = fbm(st);

    color = mix(vec3(0.101961, 0.619608, 0.666667),
                vec3(0.666667, 0.666667, 0.498039),
                clamp((f*f)*4.0, 0.0, 1.0));

    color = mix(color,
                vec3(0,0,0.164706),
                clamp(length(q), 0.0, 1.0));

    color = mix(color,
                vec3(0.666667, 1, 1),
                clamp(length(r.x), 0.0, 1.0));

    return vec3((f*f*f + .1*f*f + .1*f) * color);
}



float fog_factor(float d)
{
    const float fog_max = 70.0;
    const float fog_min = 10.0;

    if (d >= fog_max)
        return 1.;

    if (d <= fog_min)
        return 0.;

    return 1 - (fog_max - d) / (fog_max - fog_min);
}
