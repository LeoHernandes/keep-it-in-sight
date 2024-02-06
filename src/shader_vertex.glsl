#version 330 core

layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define INTERPOLATION_TYPE_GOURAUD 0
#define INTERPOLATION_TYPE_PHONG 1
uniform int interpolation_type;

out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;
out vec3 gouraud_shading_term;

void main()
{

    gl_Position = projection * view * model * model_coefficients;
    position_world = model * model_coefficients;
    position_model = model_coefficients;

    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;
    
    texcoords = texture_coefficients;


    // Gouraud shading term
    if(interpolation_type == INTERPOLATION_TYPE_GOURAUD)
    {
        vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
        vec3 Ks = vec3(0.8,0.8,0.8);
        vec4 n = normalize(normal);
        vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));
        vec4 p = position_world;
        vec4 camera_position = inverse(view) * origin;
        vec4 v = normalize(camera_position - p);
        vec4 half_vector = normalize(v + l);
        float q = 32.0;

        gouraud_shading_term =  Ks * pow(max(0, dot(n, half_vector)), q);
    }
}

