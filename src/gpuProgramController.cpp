#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->model_uniform = glGetUniformLocation(gpu_program_id, "model");
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
    this->bbox_min_uniform = glGetUniformLocation(gpu_program_id, "bbox_min");
    this->bbox_max_uniform = glGetUniformLocation(gpu_program_id, "bbox_max");
}