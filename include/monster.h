#ifndef _MONSTER_H
#define _MONSTER_H

#include "entity.h"
#include "player.h"
#include "textureLoader.h"
#include "matrices.h"
#include "collisions.h"

class Monster : public Entity
{
private:
    Player *player;

    const float VELOCITY = 2.5f;

    int texture_id;

public:
    Monster(GpuProgramController *gpu_controller, Object *object, Player *player, int texture_id, glm::mat4 model = Matrices::Identity());
    void Update(float deltaTime) override;
    void Render() override;
};

#endif