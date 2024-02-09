#include "skybox.h"

Skybox::Skybox(GpuProgramController *gpu_controller, Object *object, Player *player, int texture_id)
    : Entity(gpu_controller, Matrices::Identity(), object, LightInterpolationType::NO_LIGHT)
{
    this->texture_id = texture_id;
    this->player = player;
}

void Skybox::Update(float deltaTime)
{
    this->model = Matrices::Translate(player->position.x, player->position.y, player->position.z) * Matrices::Scale(10.0f, 10.0f, 10.0f);
}

void Skybox::Render()
{
    gpu_controller->DrawSkybox(
        object->vertex_array_object_id,
        model,
        texture_id,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}