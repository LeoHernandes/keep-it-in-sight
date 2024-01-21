#include "collisions.h"
#include "matrices.h"

std::list<HitBox*> Collisions::hitBoxes;
std::list<HitSphere*> Collisions::hitSpheres;

HitBox::HitBox(glm::vec4 rigthFrontBotton, glm::vec4 leftBackTop)
{
    this->rigthFrontBotton = rigthFrontBotton;
    this->leftBackTop = leftBackTop;
}

HitSphere::HitSphere(glm::vec4 center, float radius)
{
    this->center = center;
    this->radius = radius;
}

void Collisions::AddHitBox(HitBox* hitBox)
{
    hitBoxes.push_back(hitBox);
}

void Collisions::RemoveHitBox(HitBox* hitBox)
{
    hitBoxes.remove(hitBox);
}

void Collisions::AddHitSphere(HitSphere* hitSphere)
{
    hitSpheres.push_back(hitSphere);
}

void Collisions::RemoveHitSphere(HitSphere* hitSphere)
{
    hitSpheres.remove(hitSphere);
}

bool Collisions::CubeCubeTest(HitBox hitBox)
{
    for (HitBox* hb : hitBoxes)
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
    for (HitSphere* hitSphere : hitSpheres)
    {
        float distance = Matrices::Norm(position - hitSphere->center);
        if (distance <= hitSphere->radius) return true;
    }
    return false;
}