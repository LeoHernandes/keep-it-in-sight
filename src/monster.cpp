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
                normalized_direction_to_player_vec.z * deltaTime * VELOCITY
            );
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
    glBindVertexArray(object->vertex_array_object_id);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gpu_controller->texture_projection_type, TextureCoordinatesType::PLANE);
    glUniform1i(gpu_controller->interpolation_type, interpolation_type);
    glUniform1i(gpu_controller->texture_id, 1);
    glUniform4f(gpu_controller->bbox_min_uniform, object->bbox_min.x, object->bbox_min.y, object->bbox_min.z, 1.0f);
    glUniform4f(gpu_controller->bbox_max_uniform, object->bbox_max.x, object->bbox_max.y, object->bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, object->num_indices, GL_UNSIGNED_INT, (void *)(object->first_index * sizeof(GLuint)));
    glBindVertexArray(0);
}