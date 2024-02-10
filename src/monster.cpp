#include "monster.h"

Monster::Monster(GpuProgramController *gpu_controller, Object *object, Player *player, int texture_id, glm::mat4 model)
    : Entity(gpu_controller, model, object, LightInterpolationType::PHONG)
{
    this->texture_id = texture_id;
    this->player = player;

    this->afraid = false;
    this->time_without_stun = 0.0f;

    AudioManager::PlayAudio(AudioManager::monster_sound);
}

void Monster::VerifyAfraid()
{
    if (this->player->hit_monster_with_flash_camera)
    {
        this->player->hit_monster_with_flash_camera = false;
        
        if (time_without_stun >= STUN_RESIST_TIME)
        {
            time_without_stun = 0.0f;
            afraid = true;
            AudioManager::PlayAudio(AudioManager::fear_monster_sound);
        }
    }

    if (time_without_stun >= STUN_TIME)
        afraid = false;
}

void Monster::Update(float deltaTime)
{
    glm::vec4 direction_to_player_vec = this->player->position - glm::vec4(this->position, 1.0f);
    AudioManager::SetPositionAudio(AudioManager::monster_sound, direction_to_player_vec);
    
    if (!Matrices::IsVectorNull(direction_to_player_vec))
    {
        glm::vec4 normalized_direction_to_player_vec = Matrices::Normalize(this->player->position - glm::vec4(this->position, 1.0f));
        float normal_angle = atan2(normalized_direction_to_player_vec.x, normalized_direction_to_player_vec.z);
        float inv_angle = atan2(-normalized_direction_to_player_vec.x, -normalized_direction_to_player_vec.z);

        VerifyAfraid();

        if (afraid)
        {
            this->rotation.y = inv_angle;
            this->position -= glm::vec3(
                normalized_direction_to_player_vec.x * deltaTime * VELOCITY_AFRAID,
                0.0f,
                normalized_direction_to_player_vec.z * deltaTime * VELOCITY_AFRAID
            );
        }
        else
        {
            this->rotation.y = normal_angle;

            // Faz o monstro não ficar tão próximo do player
            if (Matrices::Norm(direction_to_player_vec) > 1.0f)
            {
                this->position += glm::vec3(
                    normalized_direction_to_player_vec.x * deltaTime * VELOCITY_ANGRY,
                    0.0f,
                    normalized_direction_to_player_vec.z * deltaTime * VELOCITY_ANGRY
                );
            }
        }
    }

    time_without_stun += deltaTime;
    // printf("time_without_stun: %f \n", time_without_stun);

    this->UpdateModel();
    this->UpdateCollision();    
}

void Monster::Render()
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