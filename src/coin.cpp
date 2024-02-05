#include "coin.h"

Coin::Coin(std::string name, GpuProgramController* gpu_controller, glm::mat4 model, Object* object, Player* player, Scene* scene)
    : Entity(name, gpu_controller, model, object)
{
    this->player = player;
    this->scene = scene;
    this->cubic_bezier_movement_coin = new CubicBezier(ANIMATION_TIME_COIN, BEZIER_P1, BEZIER_P2, BEZIER_P3, BEZIER_P4);
}

void Coin::Update(float deltaTime)
{
    this->cubic_bezier_movement_coin->Update(deltaTime);
    this->coin_movement = cubic_bezier_movement_coin->GetPoint();

    SetDeltaPosition(this->coin_movement.x, this->coin_movement.y, this->coin_movement.z);

    if (this->hit_sphere->PointSphereTest(this->player->position))
    {
        printf("oiiiiiiiiii\n");
        this->scene->RemoveEntity(this);
    }
}

void Coin::Render()
{
    glBindVertexArray(object->vertex_array_object_id);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gpu_controller->texture_projection_type, TextureProjectionType::PLANE);
    glUniform1i(gpu_controller->texture_id, 1);
    glUniform4f(gpu_controller->bbox_min_uniform, object->bbox_min.x, object->bbox_min.y, object->bbox_min.z, 1.0f);
    glUniform4f(gpu_controller->bbox_max_uniform, object->bbox_max.x, object->bbox_max.y, object->bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, object->num_indices, GL_UNSIGNED_INT, (void *)(object->first_index * sizeof(GLuint)));
    glBindVertexArray(0);
}