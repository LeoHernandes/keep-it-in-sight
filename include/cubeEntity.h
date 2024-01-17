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
    CubeEntity(std::string name, GpuProgramController *gpu_controller);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif