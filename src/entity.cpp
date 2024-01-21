#include "entity.h"

Entity::Entity(std::string name, GpuProgramController *gpu_controller, glm::mat4 model)
{
    this->name = name;
    this->gpu_controller = gpu_controller;
    this->model = model;
}
