#version 330 core

in vec4 position_world;
in vec4 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;

void main()
{
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 p = position_world;

    vec4 n = normalize(normal);

    // Direction of light source
    vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));

    vec4 v = normalize(camera_position - p);

    // Specular light
    vec4 r = -l + 2.0 * n * dot(n, l);

    vec3 Kd = vec3(0.08,0.4,0.8);
    vec3 Ks = vec3(0.8,0.8,0.8);
    vec3 Ka = vec3(0.04,0.2,0.4);
    float q = 32.0;

    // Light spectrum
    vec3 I = vec3(1.0,1.0,1.0);

    // Ambient light
    vec3 Ia = vec3(0.2,0.2,0.2);

    // Diffuse light
    vec3 lambert_diffuse_term = Kd * I * max(0, dot(n, l));

    // Ambient term
    vec3 ambient_term = Ka * Ia;

    // Specular term
    vec3 phong_specular_term  = Ks * I * pow(max(0, dot(r, v)), q);

    // Alpha component
    color.a = 1;

    color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

    // Gamma correction (sRGB monitor)
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
} 

