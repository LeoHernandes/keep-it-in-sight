#include "objEntity.h"

ObjEntity::ObjEntity(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object)
    : Entity(name, gpu_controller, model)
{
    this->object = object;

    float radius = 1.2f;
    glm::vec4 center = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    HitSphere *hs = new HitSphere(center, radius);
    Collisions::AddHitSphere(hs);
}

void ObjEntity::Update(float deltaTime)
{
}

void ObjEntity::Render()
{
    glBindVertexArray(object->vertex_array_object_id);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(gpu_controller->bbox_min_uniform, object->bbox_min.x, object->bbox_min.y, object->bbox_min.z, 1.0f);
    glUniform4f(gpu_controller->bbox_max_uniform, object->bbox_max.x, object->bbox_max.y, object->bbox_max.z, 1.0f);
    glDrawElements(GL_TRIANGLES, object->num_indices, GL_UNSIGNED_INT, (void *)(object->first_index * sizeof(GLuint)));
    glBindVertexArray(0);
}