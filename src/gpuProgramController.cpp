#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->model_uniform = glGetUniformLocation(gpu_program_id, "model");
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
    this->bbox_min_uniform = glGetUniformLocation(gpu_program_id, "bbox_min");
    this->bbox_max_uniform = glGetUniformLocation(gpu_program_id, "bbox_max");
    this->texture_projection_type = glGetUniformLocation(gpu_program_id, "texture_projection_type");
    this->texture_id = glGetUniformLocation(gpu_program_id, "texture_id");

    glUseProgram(gpu_program_id);
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage0"), 0);
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage1"), 1);
    glUniform1i(glGetUniformLocation(gpu_program_id, "TextureImage2"), 2);
    glUseProgram(0);
}