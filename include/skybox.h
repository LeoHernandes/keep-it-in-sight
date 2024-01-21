#ifndef _SKYBOX_H
#define _SKYBOX_H

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
#include "player.h"

class Skybox : public Entity
{
private:
    Player *player;

public:
    Skybox(std::string name, GpuProgramController *gpu_controller, Object *object, Player *player);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif