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
    glm::vec4 INITIAL_FOWARD_VECTOR = glm::vec4(0.0f, -1.0, 0.0f, 0.0f);
    glm::vec4 INITIAL_SIDE_VECTOR = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
    Player *player;

public:
    Camera(std::string name, GpuProgramController *gpu_controller, glm::mat4 model = Matrices::Identity(), Object *object = NULL, Player *player = NULL);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif
