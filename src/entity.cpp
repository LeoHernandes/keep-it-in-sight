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
        Matrices::RotateY(this->rotation.y) *
        Matrices::RotateZ(this->rotation.z) *
        Matrices::Translate(this->axis_position.x, this->axis_position.y, this->axis_position.z);

    switch (collision_type)
    {
        case CollisionType::HITBOX:
            this->collision.hit_box->point_min = model * glm::vec4(this->object->bbox_min, 1.0f);
            this->collision.hit_box->point_max = model * glm::vec4(this->object->bbox_max, 1.0f);
            AdjustHitboxPoints();

            break;

        case CollisionType::SPHEREBOX:
            
            break;

        case CollisionType::PLANE:
            
            break;

        case CollisionType::NOTHING:

            break;
    }
}

void Entity::AdjustHitboxPoints()
{
    float temp;
    if (collision.hit_box->point_min.x > collision.hit_box->point_max.x)
    {
        temp = collision.hit_box->point_min.x;
        collision.hit_box->point_min.x = collision.hit_box->point_max.x;
        collision.hit_box->point_max.x = temp;
    }

    if (collision.hit_box->point_min.y > collision.hit_box->point_max.y)
    {
        temp = collision.hit_box->point_min.y;
        collision.hit_box->point_min.y = collision.hit_box->point_max.y;
        collision.hit_box->point_max.y = temp;
    }

    if (collision.hit_box->point_min.z > collision.hit_box->point_max.z)
    {
        temp = collision.hit_box->point_min.z;
        collision.hit_box->point_min.z = collision.hit_box->point_max.z;
        collision.hit_box->point_max.z = temp;
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