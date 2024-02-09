#include "camera.h"

Camera::Camera(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object, Player *player)
    : Entity(name, gpu_controller, model, object)
{
    this->player = player;
}

void Camera::Update(float deltaTime)
{
    glm::vec4 player_view_vector = Matrices::Normalize(player->free_camera->view_vector);
    glm::vec4 player_side_vec = Matrices::Normalize(Matrices::CrossProduct(player->free_camera->up_vector, player_view_vector));

    glm::vec4 down_vector = Matrices::CrossProduct(player_side_vec, player_view_vector);
    glm::vec4 position = this->player->position -
                         (player_side_vec * 0.2f) +
                         (down_vector * 0.25f) +
                         (player_view_vector * 0.6f);
    glm::mat4 translation = Matrices::Translate(position.x, position.y, position.z);

    glm::mat4 scale = Matrices::Scale(0.002f, 0.002f, 0.002f);

    glm::mat4 rotate_foward = Matrices::FaceDirection(INITIAL_FOWARD_VECTOR, player_view_vector);
    glm::vec4 new_side_vector = Matrices::Normalize(rotate_foward * INITIAL_SIDE_VECTOR);
    glm::mat4 rotate_side = Matrices::FaceDirection(new_side_vector, player_side_vec);

    this->model = translation * rotate_side * rotate_foward * scale;
}

void Camera::Render()
{
    glBindVertexArray(object->vertex_array_object_id);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(gpu_controller->texture_projection_type, TextureProjectionType::PLANE);
    glUniform1i(gpu_controller->texture_id, 2);
    glUniform4f(gpu_controller->bbox_min_uniform, object->bbox_min.x, object->bbox_min.y, object->bbox_min.z, 1.0f);
    glUniform4f(gpu_controller->bbox_max_uniform, object->bbox_max.x, object->bbox_max.y, object->bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, object->num_indices, GL_UNSIGNED_INT, (void *)(object->first_index * sizeof(GLuint)));
    glBindVertexArray(0);
}
