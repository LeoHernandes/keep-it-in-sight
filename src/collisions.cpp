#include "collisions.h"
#include "matrices.h"

std::list<HitBox*> Collisions::hit_box_list;
std::list<HitSphere*> Collisions::hit_sphere_list;

HitBox::HitBox(glm::vec4 point_min, glm::vec4 point_max)
{
    this->point_min = point_min;
    this->point_max = point_max;
}

HitSphere::HitSphere(glm::vec4 center, float radius)
{
    this->center = center;
    this->radius = radius;
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

bool Collisions::CubeCubeTest(HitBox hit_box)
{
    for (HitBox* hb : hit_box_list)
    {
        // TODO: fazer a verificação
        //if () 
        //{
        //    return true;
        //}
    }
    return false;
}

bool Collisions::PointSphereTest(glm::vec4 position)
{
    for (HitSphere* hit_sphere : hit_sphere_list)
    {
        float distance = Matrices::Norm(position - hit_sphere->center);
        if (distance <= hit_sphere->radius) return true;
    }
    return false;
}

bool Collisions::PointBoxTest(glm::vec4 position)
{
    for (HitBox* hit_box : hit_box_list)
    {
        if (position.x > hit_box->point_min.x && position.x < hit_box->point_max.x &&
            position.y > hit_box->point_min.y && position.y < hit_box->point_max.y &&
            position.z > hit_box->point_min.z && position.z < hit_box->point_max.z)
        return true;
    }
    return false;
}