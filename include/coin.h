#ifndef _COIN_H
#define _COIN_H

#include "entity.h"
#include "player.h"
#include "textureLoader.h"
#include "cubicBezier.h"
#include "scene.h"
#include "lightInterpolationType.h"

class Coin : public Entity
{
private:
    Player *player;
    CubicBezier *cubic_bezier_movement_coin;
    glm::vec4 coin_movement;
    Scene *scene;
    int texture_id;

    const float ANIMATION_TIME_COIN = 3.0f;
    const glm::vec4 BEZIER_P1 = glm::vec4(0.0f, -0.3f, 0.0f, 0.0f);
    const glm::vec4 BEZIER_P2 = glm::vec4(0.5f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 BEZIER_P3 = glm::vec4(0.0f, 0.0f, -0.5f, 0.0f);
    const glm::vec4 BEZIER_P4 = glm::vec4(0.0f, 0.3f, 0.0f, 0.0f);

    const float COIN_SCALE = 0.2f;
    const float DELTA_RADIUS_COLLISION = 1.0f;

public:
    Coin(GpuProgramController *gpu_controller, Object *object, Player *player, Scene *scene, glm::vec3 position, int texture_id, glm::mat4 model = Matrices::Identity());
    void Update(float deltaTime) override;
    void Render() override;
};

#endif