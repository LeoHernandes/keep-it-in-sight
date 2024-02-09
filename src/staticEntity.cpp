#include "staticEntity.h"
#include <iostream>
#include "player.h"

StaticEntity::StaticEntity(std::string name, GpuProgramController *gpu_controller, Object *object, int texture_id, LightInterpolationType interpolation_type, glm::mat4 model)
    : Entity(name, gpu_controller, model, object, interpolation_type)
{
    this->texture_id = texture_id;
}

void StaticEntity::Update(float deltaTime)
{
}

void StaticEntity::Render()
{
    gpu_controller->DrawElements(
        object->vertex_array_object_id,
        model,
        TextureCoordinatesType::OBJ_FILE,
        interpolation_type,
        texture_id,
        object->bbox_min,
        object->bbox_max,
        object->num_indices,
        object->first_index);
}
