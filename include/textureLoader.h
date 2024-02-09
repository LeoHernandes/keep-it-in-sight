#ifndef _TEXTURELOADER_H
#define _TEXTURELOADER_H

#include "stb_image.h"
// OpenGl
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <iostream>

enum TextureCoordinatesType
{
    SPHERE,
    PLANE,
    OBJ_FILE
};

class TextureLoader
{
private:
    GLuint number_of_loaded_textures;
    std::unordered_map<std::string, unsigned int> texture_ids;

public:
    TextureLoader();
    void LoadTextureImage(const char *filename, std::string name);
    unsigned int GetTexture(std::string name);
};

#endif // _TEXTURELOADER_H