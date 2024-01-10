#include "freeCamera.h"
#include "matrices.h"

FreeCamera::FreeCamera(float screen_ratio)
{
    this->view_angle_theta = 0.0f;
    this->view_angle_phi = 0.0f;
    this->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->screen_ratio = screen_ratio;
}

void FreeCamera::Update(GLint view_uniform, GLint projection_uniform)
{
    // TODO: ta igual à camera look_at, tem q implementar ela
    float y = 1 * sin(view_angle_phi);
    float z = 1 * cos(view_angle_phi) * cos(view_angle_theta);
    float x = 1 * cos(view_angle_phi) * sin(view_angle_theta);

    glm::vec4 camera_position_c = glm::vec4(x, y, z, 1.0f);
    glm::vec4 camera_lookat_l = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 camera_view_vector = camera_lookat_l - camera_position_c;
    glm::vec4 camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::mat4 view = Matrices::CameraView(camera_position_c, camera_view_vector, camera_up_vector);

    float nearplane = -0.1f;
    float farplane = -10.0f;
    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrices::Perspective(field_of_view, screen_ratio, nearplane, farplane);

    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}