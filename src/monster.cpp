#include "monster.h"

Monster::Monster(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object, Player *player)
    : Entity(name, gpu_controller, model, object)
{
    this->player = player;
}

void Monster::Update(float deltaTime)
{
    glm::vec4 direction_to_player_vec = this->player->position - glm::vec4(this->position, 1.0f);

    if (!Matrices::IsVectorNull(direction_to_player_vec))
    {
        glm::vec4 normalized_direction_to_player_vec = Matrices::Normalize(this->player->position - glm::vec4(this->position, 1.0f));
        float angle = atan2(normalized_direction_to_player_vec.x, normalized_direction_to_player_vec.z);
        this->rotation.y = angle;

        // Não faz o monstro ficar tão próximo do player
        if (Matrices::Norm(direction_to_player_vec) > 1.0f)
        {
            this->position += glm::vec3(
                normalized_direction_to_player_vec.x * deltaTime * VELOCITY,
                0.0f,
                normalized_direction_to_player_vec.z * deltaTime * VELOCITY);
        }
    }
    this->UpdateModel();

    /////////////////// QUANDO TIVER HITBOX ///////////////////
    /*
    this->UpdateCollision();

    if (this->hit_box->PointAABBTest(this->player->position))
    {
        //printf("COLISAO COM O BIXO!\n");
    }
    */
}

void Monster::Render()
{
    gpu_controller->DrawElements(
        object->vertex_array_object_id,
        model,
        TextureCoordinatesType::SPHERE,
        interpolation_type,
        1,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}