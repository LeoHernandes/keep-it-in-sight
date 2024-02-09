#include "coin.h"

Coin::Coin(GpuProgramController *gpu_controller, Object *object, Player *player, Scene *scene, glm::vec3 position, int texture_id, glm::mat4 model)
    : Entity(gpu_controller, model, object, LightInterpolationType::GOURAUD)
{
    this->texture_id = texture_id;

    this->player = player;
    this->scene = scene;
    this->cubic_bezier_movement_coin = new CubicBezier(ANIMATION_TIME_COIN, BEZIER_P1, BEZIER_P2, BEZIER_P3, BEZIER_P4);
    this->position = position;

    this->CreateSphereBox();
    this->SetScale(COIN_SCALE, COIN_SCALE, COIN_SCALE);
    this->SetPosition(position.x, position.y, position.z);
    this->UpdateModel();

    this->hit_sphere->delta_radius = DELTA_RADIUS_COLLISION;
    this->UpdateCollision();
}

void Coin::Update(float deltaTime)
{
    this->cubic_bezier_movement_coin->Update(deltaTime);
    this->coin_movement = cubic_bezier_movement_coin->GetPoint();

    this->SetDeltaPosition(this->coin_movement.x, this->coin_movement.y, this->coin_movement.z);

    if (this->hit_sphere->PointSphereTest(this->player->position))
    {
        this->player->collected_coins += 1;
        this->scene->RemoveEntity(this);
    }

    // sem alterar a hitsphere da moeda, ou seja, não chamamos o UpdateCollision
    this->UpdateModel();
}

void Coin::Render()
{
    gpu_controller->DrawElements(
        object->vertex_array_object_id,
        model,
        TextureCoordinatesType::SPHERE,
        interpolation_type,
        texture_id,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}