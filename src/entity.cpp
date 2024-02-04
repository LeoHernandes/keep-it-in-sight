#include "entity.h"

Entity::Entity(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object)
{
    this->name = name;
    this->gpu_controller = gpu_controller;
    this->model = model;
    this->object = object;
}

void Entity::UpdateModelAndCollision()
{
    this->model =
        Matrices::Translate(this->position.x, this->position.y, this->position.z) *
        Matrices::Scale(this->scale.x, this->scale.y, this->scale.z) *
        Matrices::RotateX(this->rotation.x) *
        Matrices::RotateY(this->rotation.x) *
        Matrices::RotateZ(this->rotation.x);

    switch (collision_type)
    {
        case CollisionType::HITBOX:
            this->collision.hit_box->point_min = glm::vec4(this->object->bbox_min * this->scale + this->position, 1.0f);
            this->collision.hit_box->point_max = glm::vec4(this->object->bbox_max * this->scale + this->position, 1.0f);
            break;

        case CollisionType::SPHEREBOX:
            
            break;

        case CollisionType::PLANE:
            
            break;

        default:

            break;
    }
}

void Entity::SetPosition(float x, float y, float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    UpdateModelAndCollision();
}

void Entity::SetScale(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
    UpdateModelAndCollision();
}

void Entity::SetRotation(float x, float y, float z)
{
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
    UpdateModelAndCollision();
}

// TODO: excluir colisão antiga

void Entity::CreateHitBox()
{
    this->collision_type = CollisionType::HITBOX;

    this->collision.hit_box = new HitBox(
        glm::vec4(this->object->bbox_min, 1.0f), 
        glm::vec4(this->object->bbox_max, 1.0f)
    );
    Collisions::AddHitBox(this->collision.hit_box);
}

void Entity::CreateSphereBox()
{
    collision_type = CollisionType::SPHEREBOX;
}

void Entity::CreatePlane()
{
    collision_type = CollisionType::PLANE;
}