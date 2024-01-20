#ifndef _CUBEENTITY2_H
#define _CUBEENTITY2_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include <string>
#include "entity.h"
#include "matrices.h"
#include "cubicBezier.h"

class CubeEntity2 : public Entity
{
private:
    GLuint vertex_array_object_id;
    CubicBezier *cubicBezier;

public:
    glm::vec3 position = glm::vec3(0);

    CubeEntity2(std::string name, GpuProgramController *gpu_controller, float totalDuration, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif