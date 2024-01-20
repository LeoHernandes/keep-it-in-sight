#ifndef _CUBICBEZIER_H
#define _CUBICBEZIER_H

#include <glm/vec3.hpp>
#include <cmath>

class CubicBezier
{
private:
    float totalDuration;
    float timer;
    bool isIncreasing;

    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    glm::vec3 p4;

public:
    CubicBezier(float totalDuration, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4);
    void Update(float deltaTime);
    glm::vec3 GetPoint();
};

#endif