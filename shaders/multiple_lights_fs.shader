#version 330 core

out vec4 frag_color;


struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct Dir_Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Point_Light
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Spot_Light
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINT_LIGHTS 4

in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coords;

uniform vec3 view_pos;
uniform Dir_Light dir_light;
uniform Point_Light point_lights[NR_POINT_LIGHTS];
uniform Spot_Light spot_light;
uniform Material material;


vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(Point_Light light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 calc_spot_light(Spot_Light light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main()
{
    // Properties
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);

    // Phase 1: Directional Lighting
    vec3 result = calc_dir_light(dir_light, norm, view_dir);

    // Phase 2: Point Lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calc_point_light(point_lights[i], norm, frag_pos, view_dir);
    }

    // Phase 3: Spot Light
    result += calc_spot_light(spot_light, norm, frag_pos, view_dir);

    frag_color = vec4(result, 1.0);
}


// Calculates the color when using a directional light
vec3 calc_dir_light(Dir_Light light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);

    // Diffuse Shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular Shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Combine Results
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));

    return ambient + diffuse + specular;
}


// Calculates the color when using a point light
vec3 calc_point_light(Point_Light light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);

    // Diffuse Shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Attenuation
    float distance    = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine Results
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}


// Calculates the color when using a spot light
vec3 calc_spot_light(Spot_Light light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position - frag_pos);

    // Diffuse Shading
    float diff = max(dot(normal, light_dir), 0.0);

    // Specular Shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // Attenuation
    float distance    = length(light.position - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Spotlight Intensity
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    // Combine Results
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, tex_coords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(material.diffuse, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex_coords));
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}
