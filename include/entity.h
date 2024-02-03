#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "object.h"
#include "gpuProgramController.h"

class Entity
{
private:
    void UpdateModel();

protected:
    GpuProgramController *gpu_controller;
    glm::mat4 model;
    Object *object;
    Entity(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object);

public:
    std::string name;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
};

#endif