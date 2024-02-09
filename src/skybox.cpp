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
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(object->vertex_array_object_id);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gpu_controller->texture_projection_type, TextureCoordinatesType::SPHERE);
    glUniform1i(gpu_controller->is_texture_skybox, 1);
    glUniform1i(gpu_controller->texture_id, 0);
    glUniform4f(gpu_controller->bbox_min_uniform, object->bbox_min.x, object->bbox_min.y, object->bbox_min.z, 1.0f);
    glUniform4f(gpu_controller->bbox_max_uniform, object->bbox_max.x, object->bbox_max.y, object->bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, object->num_indices, GL_UNSIGNED_INT, (void *)(object->first_index * sizeof(GLuint)));
    glUniform1i(gpu_controller->is_texture_skybox, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}