#include "entity.h"

Entity::Entity(std::string name, GpuProgramController *gpu_controller)
{
    this->name = name;
    this->gpu_controller = gpu_controller;
}
