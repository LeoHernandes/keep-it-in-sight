#include "freeCamera.h"
#include "matrices.h"

FreeCamera::FreeCamera(float screen_ratio, GpuProgramController *gpu_controller)
{
    this->gpu_controller = gpu_controller;

    this->view_angle_theta = 3.141592 / 2;
    this->view_angle_phi = 0.0f;
    this->screen_ratio = screen_ratio;
    this->up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    this->view_vector = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

void FreeCamera::Update(glm::vec4 player_position)
{
    // View direction
    float y = sin(view_angle_phi);
    float z = cos(view_angle_phi) * cos(view_angle_theta);
    float x = cos(view_angle_phi) * sin(view_angle_theta);

    view_vector = glm::vec4(x, y, z, 0.0f);
    up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::mat4 view = Matrices::CameraView(player_position, view_vector, up_vector);

    float nearplane = -0.1f;
    float farplane = -10.0f;
    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrices::Perspective(field_of_view, screen_ratio, nearplane, farplane);

    glUniformMatrix4fv(gpu_controller->view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(gpu_controller->projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}