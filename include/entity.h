#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include <glad/glad.h>

#include "gpuProgramController.h"
class Entity
{
protected:
    GpuProgramController *gpu_controller;
    Entity(std::string name, GpuProgramController *gpu_controller);

public:
    std::string name;

    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};

#endif