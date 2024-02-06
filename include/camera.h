#ifndef _CAMERA_H
#define _CAMERA_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "textureLoader.h"
#include "entity.h"
#include "player.h"

class Camera : public Entity
{
private:
    Player *player;

public:
    Camera(std::string name, GpuProgramController *gpu_controller, glm::mat4 model = Matrices::Identity(), Object *object = NULL, Player *player = NULL);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif
