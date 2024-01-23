#ifndef _FREECAMERA_H
#define _FREECAMERA_H

#include "gpuProgramController.h"

// Matrices
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
// OpenGl
#include <glad/glad.h>

class FreeCamera
{
private:
    GpuProgramController *gpu_controller;

public:
    float view_angle_phi;   // Angle between ZX plane and Z axis
    float view_angle_theta; // Angle with respect to the Y axis
    float screen_ratio;     // User window width by height ratio
    glm::vec4 up_vector;
    glm::vec4 view_vector;

    float min_field_of_view = 3.141592 / 3.0f; // 60º
    float max_field_of_view = 3.141592 / 2.0f; // 90º
    float field_of_view = min_field_of_view;

    glm::mat4 view;
    glm::mat4 projection;

    FreeCamera(float screen_ratio, GpuProgramController *gpu_controller);
    void Update(glm::vec4 player_position);
};

#endif // _FREECAMERA_H