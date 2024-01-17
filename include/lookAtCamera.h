#ifndef _LOOKATCAMERA_H
#define _LOOKATCAMERA_H

#include "gpuProgramController.h"

// Matrices
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
// OpenGl
#include <glad/glad.h>

class LookAtCamera
{
private:
    GpuProgramController *gpu_controller;

public:
    float view_angle_phi;   // Angle between ZX plane and Z axis
    float view_angle_theta; // Angle with respect to the Y axis
    float distance;         // Distance to look at point
    float screen_ratio;     // User window width by height ratio

    glm::mat4 view;
    glm::mat4 projection;

    LookAtCamera(float screen_ratio, GpuProgramController *gpu_controller);
    void Update(glm::vec4 player_position);
};

#endif // _LOOKATCAMERA_H