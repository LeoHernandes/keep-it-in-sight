#ifndef _COIN_H
#define _COIN_H

#include "entity.h"
#include "player.h"
#include "textureLoader.h"
#include "cubicBezier.h"
#include "scene.h"

class Coin : public Entity
{
private:
    Player*player;
    CubicBezier* cubic_bezier_movement_coin;
    glm::vec4 coin_movement;
    Scene* scene;

    const float ANIMATION_TIME_COIN = 0.5f;
    const glm::vec4 BEZIER_P1 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 BEZIER_P2 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 BEZIER_P3 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    const glm::vec4 BEZIER_P4 = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

public:
    Coin(std::string name, GpuProgramController *gpu_controller, glm::mat4 model = Matrices::Identity(), Object *object = NULL, Player *player = NULL, Scene *scene = NULL);
    void Update(float deltaTime) override;
    void Render() override;
};

#endif