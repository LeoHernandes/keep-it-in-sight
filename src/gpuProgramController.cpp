#include "gpuProgramController.h"

GpuProgramController::GpuProgramController(GLuint gpu_program_id)
{
    this->model_uniform = glGetUniformLocation(gpu_program_id, "model");
    this->view_uniform = glGetUniformLocation(gpu_program_id, "view");
    this->projection_uniform = glGetUniformLocation(gpu_program_id, "projection");
}