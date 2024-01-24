#ifndef _CUBICBEZIER_H
#define _CUBICBEZIER_H

#include <glm/vec4.hpp>

class CubicBezier
{
private:
    float totalDuration;
    float timer;
    bool isIncreasing;

    glm::vec4 p1;
    glm::vec4 p2;
    glm::vec4 p3;
    glm::vec4 p4;

public:
    CubicBezier(float totalDuration, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);
    void Update(float deltaTime);
    glm::vec4 GetPoint();
};

#endif