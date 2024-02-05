#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <glm/vec4.hpp>
#include <list>

class HitBox;
class HitSphere;

enum class CollisionType 
{
    NOTHING,
    HITBOX,
    SPHEREBOX
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

        static bool PlayerSphereTest(glm::vec4 player_position);
        static bool PlayerBoxTest(glm::vec4 player_position);
};

class HitBox
{
public:
    glm::vec4 point_min;
    glm::vec4 point_max;

    HitBox(glm::vec4 point_min, glm::vec4 point_max);
    bool PointAABBTest(glm::vec4 position);
    bool RayAABBTest(glm::vec4 ray_origin, glm::vec4 ray_direction, float max_distance);
};

class HitSphere
{
public:
    glm::vec4 center;
    float radius;
    float original_radius;
    float delta_radius;

    HitSphere(glm::vec4 center, float radius);
    bool PointSphereTest(glm::vec4 position);
};

#endif