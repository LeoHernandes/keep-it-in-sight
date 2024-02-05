#include "entity.h"
#include <iostream>

Entity::Entity(std::string name, GpuProgramController *gpu_controller, glm::mat4 model, Object *object)
{
    this->name = name;
    this->gpu_controller = gpu_controller;
    this->model = model;
    this->object = object;
}

void Entity::UpdateModel()
{
    this->model =
        Matrices::Translate(this->position.x + this->delta_position.x, this->position.y + this->delta_position.y, this->position.z + this->delta_position.z) *
        Matrices::Scale(this->scale.x, this->scale.y, this->scale.z) *
        Matrices::RotateX(this->rotation.x) *
        Matrices::RotateY(this->rotation.y) *
        Matrices::RotateZ(this->rotation.z) *
        Matrices::Translate(this->axis_position.x, this->axis_position.y, this->axis_position.z);
}

void Entity::UpdateCollision()
{
    switch (collision_type)
    {
        case CollisionType::HITBOX:
            this->hit_box->point_min = model * glm::vec4(this->object->bbox_min, 1.0f);
            this->hit_box->point_max = model * glm::vec4(this->object->bbox_max, 1.0f);
            AdjustHitboxPoints();

            break;

        case CollisionType::SPHEREBOX:
            this->hit_sphere->center = glm::vec4(this->position + this->delta_position, 1.0f);
            this->hit_sphere->radius = this->hit_sphere->original_radius * this->scale.x;

            //std::cout << "===================== " + this->name + " =====================" << std::endl;
            //printf("center | x: %f, y: %f, z: %f \n", hit_sphere->center.x, hit_sphere->center.y, hit_sphere->center.z);
            //printf("radius | r: %f \n\n", hit_sphere->radius + hit_sphere->delta_radius);

            break;

        case CollisionType::NOTHING:

            break;
    }
}

void Entity::AdjustHitboxPoints()
{
    float temp;
    if (hit_box->point_min.x > hit_box->point_max.x)
    {
        temp = hit_box->point_min.x;
        hit_box->point_min.x = hit_box->point_max.x;
        hit_box->point_max.x = temp;
    }

    if (hit_box->point_min.y > hit_box->point_max.y)
    {
        temp = hit_box->point_min.y;
        hit_box->point_min.y = hit_box->point_max.y;
        hit_box->point_max.y = temp;
    }

    if (hit_box->point_min.z > hit_box->point_max.z)
    {
        temp = hit_box->point_min.z;
        hit_box->point_min.z = hit_box->point_max.z;
        hit_box->point_max.z = temp;
    }
}

void Entity::SetPosition(float x, float y, float z)
{
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
}

void Entity::SetScale(float x, float y, float z)
{
    this->scale.x = x;
    this->scale.y = y;
    this->scale.z = z;
}

void Entity::SetRotation(float x, float y, float z)
{
    this->rotation.x = x;
    this->rotation.y = y;
    this->rotation.z = z;
}

void Entity::SetDeltaPosition(float x, float y, float z)
{
    this->delta_position.x = x;
    this->delta_position.y = y;
    this->delta_position.z = z;
}

// TODO: excluir colisão antiga

void Entity::CreateHitBox()
{
    this->collision_type = CollisionType::HITBOX;

    this->hit_box = new HitBox(
        glm::vec4(this->object->bbox_min, 1.0f), 
        glm::vec4(this->object->bbox_max, 1.0f)
    );
    Collisions::AddHitBox(this->hit_box);
}

void Entity::CreateSphereBox()
{
    collision_type = CollisionType::SPHEREBOX;

    glm::vec3 center = (this->object->bbox_min + this->object->bbox_max) / 2.0f;
    float radius = center.x - this->object->bbox_min.x;

    this->hit_sphere = new HitSphere(
        glm::vec4(center, 1.0f), 
        radius
    );
    Collisions::AddHitSphere(this->hit_sphere);
}
