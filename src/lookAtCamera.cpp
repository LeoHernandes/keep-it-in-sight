#include "lookAtCamera.h"
#include "matrices.h"

LookAtCamera::LookAtCamera(float screen_ratio, GpuProgramController *gpu_controller)
{
    this->gpu_controller = gpu_controller;

    this->view_angle_theta = 0.0f;
    this->view_angle_phi = 0.0f;
    this->distance = 3.5f;
    this->screen_ratio = screen_ratio;
}

void LookAtCamera::Update(glm::vec4 player_position)
{
    // Look at camera position
    float r = distance;
    float y = r * sin(view_angle_phi);
    float z = r * cos(view_angle_phi) * cos(view_angle_theta);
    float x = r * cos(view_angle_phi) * sin(view_angle_theta);

    glm::vec4 camera_position_c = glm::vec4(x, y, z, 1.0f) + Matrices::Vectorize(player_position);
    glm::vec4 camera_view_vector = player_position - camera_position_c;
    glm::vec4 camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::mat4 view = Matrices::CameraView(camera_position_c, camera_view_vector, camera_up_vector);

    float nearplane = -0.1f;
    float farplane = -10.0f;
    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrices::Perspective(field_of_view, screen_ratio, nearplane, farplane);

    glUniformMatrix4fv(gpu_controller->view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(gpu_controller->projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}