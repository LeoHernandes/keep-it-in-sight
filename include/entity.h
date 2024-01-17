#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include <glad/glad.h>

class Entity
{
public:
    std::string name;

    virtual void Update(float deltaTime);
    virtual void Render(GLint model_uniform);
};

#endif