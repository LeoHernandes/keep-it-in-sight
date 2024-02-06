#include "camera.h"

Camera::Camera(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object, Player *player)
    : Entity(name, gpu_controller, model, object)
{
    this->player = player;
}

void Camera::Update(float deltaTime)
{
    /*
    //this->position = this->player->position + this->player->free_camera->view_vector; //glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    
    glm::mat4 inverseViewMatrix = glm::inverse(this->player->free_camera->view);
    glm::vec4 worldPosition = inverseViewMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    //glm::vec4 worldPosition = inverseViewMatrix * glm::vec4(relativePosition, 1.0f);

    this->position = glm::vec3(worldPosition);

    this->UpdateModel();
    */
}

void Camera::Render()
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
