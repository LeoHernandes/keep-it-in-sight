#version 330 core

in vec4 position_world;
in vec4 position_model;
in vec4 normal;
in vec2 texcoords;
in vec3 gouraud_shading_term;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Which type of light interpolation should use
#define INTERPOLATION_TYPE_GOURAUD 0
#define INTERPOLATION_TYPE_PHONG 1
#define INTERPOLATION_TYPE_NO_LIGHT 2
uniform int interpolation_type;

// Which texture projection should use for this object
#define TEXTURE_PROJECTION_SPHERE 0
#define TEXTURE_PROJECTION_PLANE 1
uniform int texture_projection_type;

#define TEXTURE_SKYBOX 0
uniform int texture_id;

uniform sampler2D TextureImage0;
uniform sampler2D TextureImage1;
uniform sampler2D TextureImage2;
uniform sampler2D TextureImage3;
uniform sampler2D TextureImage4;
uniform sampler2D TextureImage5;

out vec4 color;

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

struct TextureCoordinates
{
    float u;
    float v;
};

TextureCoordinates GetTextureCoordinatesFromSphereProjection(vec4 bbox_min, vec4 bbox_max, vec4 position_model)
{
    vec4 bbox_center = (bbox_min + bbox_max) / 2.0;

    float theta = atan(position_model.x, position_model.z);
    float phi = asin(position_model.y/length(position_model - bbox_center));

    float u = (theta + M_PI)/(2*M_PI);
    float v = (phi + M_PI_2)/M_PI;

    return TextureCoordinates(u, v);
}

TextureCoordinates GetTextureCoordinatesFromPlaneProjection(vec4 bbox_min, vec4 bbox_max, vec4 position_model)
{
    float minx = bbox_min.x;
    float maxx = bbox_max.x;

    float miny = bbox_min.y;
    float maxy = bbox_max.y;

    float minz = bbox_min.z;
    float maxz = bbox_max.z;

    float u = (position_model.x - minx)/(maxx - minx);
    float v = (position_model.y - miny)/(maxy - miny);

    return TextureCoordinates(u, v);
}

vec3 GetTextureColorFromTextureId(int texture_id, TextureCoordinates text_coords)
{
    if(texture_id == 1)
    {
        return texture(TextureImage1, vec2(texcoords[0],texcoords[1])).rgb;
    }
    else if(texture_id == 2)
    {
        return texture(TextureImage2, vec2(texcoords[0],texcoords[1])).rgb;
    }
    else if(texture_id == 3)
    {
        return texture(TextureImage3, vec2(texcoords[0],texcoords[1])).rgb;
    }
    else if(texture_id == 4)
    {
        return texture(TextureImage4, vec2(1*texcoords[0],10*texcoords[1])).rgb;
    }
    else if(texture_id == 5)
    {
        return texture(TextureImage5, vec2(1.5*texcoords[0],1.5*texcoords[1])).rgb;
    }
    else
    {
        return vec3(0.0, 0.0, 0.0);
    }
}

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

    TextureCoordinates text_coords;
    if ( texture_projection_type == TEXTURE_PROJECTION_SPHERE )
    {
        text_coords = GetTextureCoordinatesFromSphereProjection(bbox_min, bbox_max, position_model);
    }
    else if ( texture_projection_type == TEXTURE_PROJECTION_PLANE )
    {
        text_coords = GetTextureCoordinatesFromPlaneProjection(bbox_min, bbox_max, position_model);
    }
    else
    {
        text_coords = TextureCoordinates(0.0, 0.0);
    }

    if(texture_id == TEXTURE_SKYBOX)
    {
        color.rgb = texture(TextureImage0, vec2(text_coords.u,text_coords.v)).rgb;
    }
    else
    {
        vec3 Kd0 = GetTextureColorFromTextureId(texture_id, text_coords);
        vec3 Ks = vec3(0.8,0.8,0.8);
        float q = 32.0;

        // Diffuse light
        vec3 lambert_diffuse_term = Kd0 * (max(0, dot(n, l)) + 0.01);

        // Specular term (Blinn-Phong)          
        vec4 half_vector = normalize(v + l);
        vec3 phong_specular_term  = Ks * pow(max(0, dot(n, half_vector)), q);

        // Alpha component
        color.a = 1;

        if(interpolation_type == INTERPOLATION_TYPE_GOURAUD)
        {
            color.rgb = lambert_diffuse_term + gouraud_shading_term;
        }
        else if ( interpolation_type == INTERPOLATION_TYPE_PHONG)
        {
            color.rgb = lambert_diffuse_term + phong_specular_term;
        }
        else 
        {
            color.rgb = lambert_diffuse_term;
        }
    }

    // Gamma correction (sRGB monitor)
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
} 

