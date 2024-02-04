#include "collisions.h"
#include "matrices.h"

std::list<HitBox*> Collisions::hit_box_list;
std::list<HitSphere*> Collisions::hit_sphere_list;

HitBox::HitBox(glm::vec4 point_min, glm::vec4 point_max)
{
    this->point_min = point_min;
    this->point_max = point_max;
}

bool HitBox::PointAABBTest(glm::vec4 position)
{
    if (position.x > this->point_min.x && position.x < this->point_max.x &&
        position.y > this->point_min.y && position.y < this->point_max.y &&
        position.z > this->point_min.z && position.z < this->point_max.z)
        return true;

    return false;
}

// FONTE: https://tavianator.com/2011/ray_box.html
bool HitBox::RayAABBTest(glm::vec4 ray_origin, glm::vec4 ray_direction, float max_distance)
{
    glm::vec4 inv_direction = 1.0f / ray_direction;

    glm::vec4 t_min = (this->point_min - ray_origin) * inv_direction;
    glm::vec4 t_max = (this->point_max - ray_origin) * inv_direction;

    glm::vec4 t1 = glm::min(t_min, t_max);
    glm::vec4 t2 = glm::max(t_min, t_max);

    float t_near = glm::max(t1.x, glm::max(t1.y, t1.z));
    float t_far = glm::min(t2.x, glm::min(t2.y, t2.z));

    return t_near <= t_far && t_near <= max_distance;
}

HitSphere::HitSphere(glm::vec4 center, float radius)
{
    this->center = center;
    this->radius = radius;
}

bool HitSphere::PointSphereTest(glm::vec4 position)
{
    float distance = Matrices::Norm(position - this->center);
    if (distance <= this->radius) return true;

    return false;
}

void Collisions::AddHitBox(HitBox* hit_box)
{
    hit_box_list.push_back(hit_box);
}

void Collisions::RemoveHitBox(HitBox* hit_box)
{
    hit_box_list.remove(hit_box);
}

void Collisions::AddHitSphere(HitSphere* hit_sphere)
{
    hit_sphere_list.push_back(hit_sphere);
}

void Collisions::RemoveHitSphere(HitSphere* hit_sphere)
{
    hit_sphere_list.remove(hit_sphere);
}

bool Collisions::PlayerBoxTest(glm::vec4 player_position)
{
    for (HitBox* hit_box : hit_box_list)
       if (hit_box->PointAABBTest(player_position)) return true;

    return false;
}

bool Collisions::PlayerSphereTest(glm::vec4 player_position)
{
    for (HitSphere* hit_sphere : hit_sphere_list)
        if (hit_sphere->PointSphereTest(player_position)) return true;

    return false;
}