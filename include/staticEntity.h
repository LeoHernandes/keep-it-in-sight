#ifndef _STATICENTITY_H
#define _STATICENTITY_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>

#include <cassert>
#include <string>
#include "entity.h"
#include "matrices.h"
#include "collisions.h"
#include "object.h"
#include "textureLoader.h"

class StaticEntity : public Entity
{
private:
    int texture_id;

public:
    StaticEntity(GpuProgramController *gpu_controller, Object *object, int texture_id, LightInterpolationType interpolation_type, glm::mat4 model = Matrices::Identity());
    void Update(float deltaTime) override;
    void Render() override;
};

#endif