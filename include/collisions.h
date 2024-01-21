#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <glm/vec4.hpp>
#include <list>

class HitBox;
class HitSphere;

class Collisions
{
    private:
        Collisions();

        static std::list<HitBox*> hitBoxes;
        static std::list<HitSphere*> hitSpheres;

    public:
        static void AddHitBox(HitBox* hitBox);
        static void RemoveHitBox(HitBox* hitBox);
        static void AddHitSphere(HitSphere* hitSphere);
        static void RemoveHitSphere(HitSphere* hitSphere);

        static bool CubeCubeTest(HitBox hitBox);
        static bool PointSphereTest(glm::vec4 position);
        // TODO: fazer outros tipos de testes de colisão
};

class HitBox
{
private:
    glm::vec4 rigthFrontBotton;
    glm::vec4 leftBackTop;

public:
    HitBox(glm::vec4 rigthFrontBotton, glm::vec4 leftBackTop);

    // this function can get private attributes because of reserved word 'friend'
    friend bool Collisions::CubeCubeTest(HitBox hitBox);
};

class HitSphere
{
private:
    glm::vec4 center;
    float radius;

public:
    HitSphere(glm::vec4 center, float radius);

    // this function can get private attributes because of reserved word 'friend'
    friend bool Collisions::PointSphereTest(glm::vec4 position);
};

#endif