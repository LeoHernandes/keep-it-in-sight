#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->model_uniform = glGetUniformLocation(gpu_program_id, "model");
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
    this->bbox_min_uniform = glGetUniformLocation(gpu_program_id, "bbox_min");
    this->bbox_max_uniform = glGetUniformLocation(gpu_program_id, "bbox_max");
    this->texture_coordinates_type = glGetUniformLocation(gpu_program_id, "texture_coordinates_type");
    this->interpolation_type = glGetUniformLocation(gpu_program_id, "interpolation_type");
    this->is_texture_skybox = glGetUniformLocation(gpu_program_id, "is_texture_skybox");
    this->texture_id = glGetUniformLocation(gpu_program_id, "texture_id");
}

void GpuProgramController::DrawElements(GLuint VAO_id, glm::mat4 model, TextureCoordinatesType text_coords_type, LightInterpolationType light_type, GLint tex_id, glm::vec3 bbox_min, glm::vec3 bbox_max, size_t num_indices, size_t first_index)
{
    glBindVertexArray(VAO_id);
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(texture_coordinates_type, text_coords_type);
    glUniform1i(interpolation_type, light_type);
    glUniform1i(texture_id, tex_id);
    glUniform4f(bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void *)(first_index * sizeof(GLuint)));
    glBindVertexArray(0);
}

void GpuProgramController::DrawSkybox(GLuint VAO_id, glm::mat4 model, GLint tex_id, glm::vec3 bbox_min, glm::vec3 bbox_max, size_t num_indices, size_t first_index)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(VAO_id);
    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1i(texture_coordinates_type, TextureCoordinatesType::SPHERE);
    glUniform1i(is_texture_skybox, 1);
    glUniform1i(texture_id, 0);
    glUniform4f(bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, (void *)(first_index * sizeof(GLuint)));
    glUniform1i(is_texture_skybox, 0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void GpuProgramController::SendPlayerCameraMatrices(glm::mat4 view, glm::mat4 projection)
{
    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}
