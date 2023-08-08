#version 330 core


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
uniform vec3 view_pos;

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coords;


#define NUM_OCTAVES 5


vec3 generate_noise();
float random(vec2 _st);
float noise(in vec2 _st);
float fbm (vec2 _st);
vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir, vec3 noise);


void main()
{
    // Properties
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    // Phase 1: Directional Lighting
    vec3 noise = generate_noise();
    vec3 result = calc_dir_light(dir_light, norm, view_dir, noise);

    frag_color = vec4(result, 1.0);
}



vec3 generate_noise()
{
    vec2 st = gl_FragCoord.xy/u_resolution.xy*3;

    vec3 color = vec3(0.0);

    //color += fbm(st*3.0);

    vec2 q = vec2(0.);
    //q.x = fbm(st + 0.00*u_time);
    //q.y = fbm(st + vec2(1.0));

    q.x = fbm(st + vec2(0.0, 0.0));
    q.y = fbm(st + vec2(5.2, 1.3));

    vec2 r = vec2(0.);
    r.x = fbm(st + 1.0*q + vec2(1.7, 9.2) + u_time);
    r.y = fbm(st + 1.0*q + vec2(8.3, 2.8) + u_time);

    float f = fbm(st + 1.0*r);

    color = mix(vec3(0.101961, 0.619608, 0.666667),
                vec3(0.666667, 0.666667, 0.498039),
                clamp((f*f)*4.0, 0.0, 1.0));

    color = mix(color,
                vec3(0,0,0.164706),
                clamp(length(q), 0.0, 1.0));

    color = mix(color,
                vec3(0.666667, 1, 1),
                clamp(length(r.x), 0.0, 1.0));

    //color = vec3(0.8, 0.8, 1.0);
    return vec3(f*f*f, f*f, f) * color;
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
    float value = 0.0;
    float amplitude = 0.5;

    for (int i = 0; i < NUM_OCTAVES; ++i)
    {
        value += amplitude * noise(_st);
        _st *= 2.;
        amplitude *= 0.5;
    }

    return value;
}



vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir, vec3 noise)
{
    vec3 light_dir = normalize(-light.direction);

    // Diffuse Shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular Shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float shininess = 32.0;
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    // Combine Results
    vec3 ambient  = light.ambient * noise;
    vec3 diffuse  = light.diffuse * diff * noise;
    vec3 specular = light.specular * spec * noise;

    return ambient + diffuse + specular;
}
