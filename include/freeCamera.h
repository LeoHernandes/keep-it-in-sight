#ifndef _FREECAMERA_H
#define _FREECAMERA_H

// Matrices
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
// OpenGl
#include <glad/glad.h>

class FreeCamera
{
public:
    float view_angle_phi;   // Angle between ZX plane and Z axis
    float view_angle_theta; // Angle with respect to the Y axis
    float screen_ratio;     // User window width by height ratio
    glm::vec4 position;

    glm::mat4 view;
    glm::mat4 projection;

    FreeCamera(float screen_ratio);
    void Update(GLint view_uniform, GLint projection_uniform);
};

#endif // _FREECAMERA_H