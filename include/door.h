#ifndef _DOOR_H
#define _DOOR_H

#include "entity.h"
#include "player.h"
#include "textureLoader.h"

class Door : public Entity
{
private:
    Player *player;

    bool is_opening;
    bool is_closing;
    float progression_time;

    const float ANIMATION_TIME = 0.5f;

public:
    Door(std::string name, GpuProgramController *gpu_controller, glm::mat4 model = Matrices::Identity(), Object *object = NULL, Player *player = NULL);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif