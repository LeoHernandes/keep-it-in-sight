#ifndef _TEXTURELOADER_H
#define _TEXTURELOADER_H

#include "stb_image.h"
// OpenGl
#include <glad/glad.h>

class TextureLoader
{
private:
    GLuint number_of_loaded_textures;

public:
    TextureLoader();
    void LoadTextureImage(const char *filename);
};

#endif // _TEXTURELOADER_H