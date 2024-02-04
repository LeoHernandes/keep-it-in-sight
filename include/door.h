#ifndef _DOOR_H
#define _DOOR_H

#include "entity.h"
#include "textureLoader.h"

class Door : public Entity
{
public:
    Door(std::string name, GpuProgramController *gpu_controller, glm::mat4 model = Matrices::Identity(), Object *object = NULL);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif