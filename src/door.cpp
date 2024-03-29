#include "door.h"

Door::Door(GpuProgramController *gpu_controller, Object *object, Player *player, int texture_id, glm::mat4 model)
    : Entity(gpu_controller, model, object, LightInterpolationType::PHONG)
{
    this->texture_id = texture_id;
    this->player = player;

    this->axis_position.x = -(this->object->bbox_min.x + this->object->bbox_min.x) / 2;

    this->is_opening = false;
    this->is_closing = false;
    this->progression_time = 0.0f;

    AudioManager::SetAudioSpeed(AudioManager::door_sound, 1/ANIMATION_TIME);
}

void Door::Update(float deltaTime)
{
    if (this->player->_is_pressing_E_key && this->hit_box->RayAABBTest(player->position, player->free_camera->view_vector, MAX_DISTANCE_TO_OPEN_DOOR))
    {
        if (this->progression_time <= ANIMATION_TIME && !is_closing)
            this->is_opening = true;
        else
            this->is_closing = true;
    }

    if (this->is_opening)
        OpeningDoor(deltaTime);
    else if (this->is_closing)
        ClosingDoor(deltaTime);

    this->UpdateModel();
    this->UpdateCollision();
}

void Door::OpeningDoor(float deltaTime)
{
    if (this->progression_time >= ANIMATION_TIME)
        this->is_opening = false;

    this->progression_time += deltaTime;
    this->rotation.z = (progression_time / ANIMATION_TIME) * DOOR_OPENING_ANGLE;
    this->SetRotation(this->rotation.x, this->rotation.y, this->rotation.z);

    AudioManager::PlayAudio(AudioManager::door_sound);
}

void Door::ClosingDoor(float deltaTime)
{
    if (this->progression_time <= 0.0f)
        this->is_closing = false;

    this->progression_time -= deltaTime;
    this->rotation.z = (progression_time / ANIMATION_TIME) * (3.141592 / 2);
    this->SetRotation(this->rotation.x, this->rotation.y, this->rotation.z);

    AudioManager::PlayAudio(AudioManager::door_sound);
}

void Door::Render()
{
    gpu_controller->DrawElements(
        object->vertex_array_object_id,
        model,
        TextureCoordinatesType::OBJ_FILE,
        interpolation_type,
        texture_id,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}