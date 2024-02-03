#include "staticEntity.h"
#include <iostream>
#include "player.h"

StaticEntity::StaticEntity(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object)
    : Entity(name, gpu_controller, model, object)
{
    hit_box = new HitBox(
        glm::vec4(this->object->bbox_min, 1.0f), 
        glm::vec4(this->object->bbox_max, 1.0f)
    );
    Collisions::AddHitBox(hit_box);
}

void StaticEntity::Update(float deltaTime)
{

}

void StaticEntity::Render()
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

void StaticEntity::UpdateModelAndHitBox()
{
    this->model =
        Matrices::Translate(this->position.x, this->position.y, this->position.z) *
        Matrices::Scale(this->scale.x, this->scale.y, this->scale.z) *
        Matrices::RotateX(this->rotation.x) *
        Matrices::RotateY(this->rotation.x) *
        Matrices::RotateZ(this->rotation.x);

    this->hit_box->point_min = glm::vec4(this->object->bbox_min * this->scale + this->position, 1.0f);
    this->hit_box->point_max = glm::vec4(this->object->bbox_max * this->scale + this->position, 1.0f);
}

void StaticEntity::SetPosition(float x, float y, float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    UpdateModelAndHitBox();
}

void StaticEntity::SetScale(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
    UpdateModelAndHitBox();
}

void StaticEntity::SetRotation(float x, float y, float z)
{
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
    UpdateModelAndHitBox();
}