#ifndef _MONSTER_H
#define _MONSTER_H

#include "entity.h"
#include "player.h"
#include "textureLoader.h"
#include "matrices.h"

class Monster : public Entity
{
private:
    Player *player;

    const float VELOCITY = 2.5f;

public:
    Monster(std::string name, GpuProgramController *gpu_controller, glm::mat4 model = Matrices::Identity(), Object *object = NULL, Player *player = NULL);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif