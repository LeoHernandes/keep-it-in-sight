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
    int texture_id;

    float time_without_stun;
    bool afraid;

    const float VELOCITY_ANGRY = 2.5f;
    const float VELOCITY_AFRAID = 5.0f;
    const float STUN_TIME = 1.5f;
    const float STUN_RESIST_TIME = 5.0f;

    void VerifyAfraid();

public:
    Monster(GpuProgramController *gpu_controller, Object *object, Player *player, int texture_id, glm::mat4 model = Matrices::Identity());
    void Update(float deltaTime) override;
    void Render() override;
};

#endif