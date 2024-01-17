#ifndef _CUBEENTITY_H
#define _CUBEENTITY_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "entity.h"
#include "matrices.h"

class CubeEntity : public Entity
{
private:
    GLuint vertex_array_object_id;

public:
    std::string name;

    CubeEntity(std::string name);
    void Update(float deltaTime);
    void Render(GLint model_uniform);
};

#endif