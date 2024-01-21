#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <glm/vec3.hpp>
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
        // TODO: fazer outros tipos de testes de colisão
};

class HitBox
{
private:
    glm::vec3 rigthFrontBotton;
    glm::vec3 leftBackTop;

public:
    HitBox(glm::vec3 rigthFrontBotton, glm::vec3 leftBackTop);

    // this function can get private attributes because of reserved word 'friend'
    friend bool Collisions::CubeCubeTest(HitBox hitBox);
};

class HitSphere
{
private:
    glm::vec3 center;
    float radius;

public:
    HitSphere(glm::vec3 center, float radius);
};

#endif