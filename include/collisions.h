#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <glm/vec4.hpp>
#include <list>

class HitBox;
class HitSphere;
class Plane;

union CollisionUnion
{
    HitBox *hit_box;
    HitSphere *hit_sphere;
    Plane *plane;
};

enum class CollisionType 
{
    NOTHING,
    HITBOX,
    SPHEREBOX,
    PLANE
};

class Collisions
{
    private:
        Collisions();

        static std::list<HitBox*> hit_box_list;
        static std::list<HitSphere*> hit_sphere_list;

    public:
        static void AddHitBox(HitBox* hit_box);
        static void RemoveHitBox(HitBox* hit_box);
        static void AddHitSphere(HitSphere* hitSphere);
        static void RemoveHitSphere(HitSphere* hitSphere);

        static bool CubeCubeTest(HitBox hit_box);
        static bool PointSphereTest(glm::vec4 position);
        static bool PointBoxTest(glm::vec4 position);
        // TODO: fazer outros tipos de testes de colisão
};

class HitBox
{
public:
    glm::vec4 point_min;
    glm::vec4 point_max;

    HitBox(glm::vec4 point_min, glm::vec4 point_max);
};

class HitSphere
{
public:
    glm::vec4 center;
    float radius;

    HitSphere(glm::vec4 center, float radius);
};

class Plane
{
private:

public:

};

#endif