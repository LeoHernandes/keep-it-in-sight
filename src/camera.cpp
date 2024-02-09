#include "camera.h"

Camera::Camera(GpuProgramController *gpu_controller, Object *object, Player *player, glm::mat4 model)
    : Entity(gpu_controller, model, object, LightInterpolationType::PHONG)
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
    gpu_controller->DrawElements(
        object->vertex_array_object_id,
        model,
        TextureCoordinatesType::OBJ_FILE,
        interpolation_type,
        2,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}
