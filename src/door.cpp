#include "door.h"

Door::Door(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object, Player *player)
    : Entity(name, gpu_controller, model, object)
{
    this->player = player;

    this->axis_position.x = -(this->object->bbox_min.x + this->object->bbox_min.x) / 2;

    this->is_opening = false;
    this->is_closing = false;
    this->progression_time = 0.0f;
}

void Door::Update(float deltaTime)
{
    if (this->player->_is_pressing_E_key && this->collision.hit_box->RayAABBTest(player->position, player->free_camera->view_vector, MAX_DISTANCE_TO_OPEN_DOOR))
    {
        if (this->progression_time <= ANIMATION_TIME)
            this->is_opening = true;
        else
            this->is_closing = true;
    }

    if (this->is_opening)
        OpeningDoor(deltaTime);
    else if (this->is_closing)
        ClosingDoor(deltaTime);
}

void Door::OpeningDoor(float deltaTime)
{
    if (this->progression_time >= ANIMATION_TIME)
            this->is_opening = false;

        this->progression_time += deltaTime;
        this->rotation.z = (progression_time / ANIMATION_TIME) * DOOR_OPENING_ANGLE;
        this->SetRotation(this->rotation.x, this->rotation.y, this->rotation.z);
}

void Door::ClosingDoor(float deltaTime)
{
    if (this->progression_time <= 0.0f)
            this->is_closing = false;

        this->progression_time -= deltaTime;
        this->rotation.z = (progression_time / ANIMATION_TIME) * (3.141592 / 2);
        this->SetRotation(this->rotation.x, this->rotation.y, this->rotation.z);
}

void Door::Render()
{
    glBindVertexArray(object->vertex_array_object_id);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gpu_controller->texture_projection_type, TextureProjectionType::SPHERE);
    glUniform1i(gpu_controller->texture_id, 1);
    glUniform4f(gpu_controller->bbox_min_uniform, object->bbox_min.x, object->bbox_min.y, object->bbox_min.z, 1.0f);
    glUniform4f(gpu_controller->bbox_max_uniform, object->bbox_max.x, object->bbox_max.y, object->bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, object->num_indices, GL_UNSIGNED_INT, (void *)(object->first_index * sizeof(GLuint)));
    glBindVertexArray(0);
}