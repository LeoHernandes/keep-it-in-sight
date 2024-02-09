#include "skybox.h"

Skybox::Skybox(std::string name, GpuProgramController *gpu_controller, Object *object, Player *player)
    : Entity(name, gpu_controller, Matrices::Identity(), object)
{
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
        0,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}