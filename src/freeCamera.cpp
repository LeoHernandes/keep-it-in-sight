#include "freeCamera.h"
#include "matrices.h"

FreeCamera::FreeCamera(float screen_ratio)
{
    this->view_angle_theta = 3.141592 / 2;
    this->view_angle_phi = 0.0f;
    this->position = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    this->screen_ratio = screen_ratio;
}

void FreeCamera::Update(GLint view_uniform, GLint projection_uniform, bool is_pressing_W, bool is_pressing_A, bool is_pressing_S, bool is_pressing_D)
{
    // View direction
    float y = sin(view_angle_phi);
    float z = cos(view_angle_phi) * cos(view_angle_theta);
    float x = cos(view_angle_phi) * sin(view_angle_theta);

    glm::vec4 camera_look_at_point = glm::vec4(x + position.x, y + position.y, z + position.z, 1.0f);
    glm::vec4 camera_view_vector = camera_look_at_point - position;
    glm::vec4 camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    // Camera position
    glm::vec4 foward_vector = camera_view_vector;
    foward_vector.y = 0.0f;

    glm::vec4 camera_side_vec = Matrices::CrossProduct(camera_up_vector, foward_vector);
    glm::vec4 normalized_side = camera_side_vec / Matrices::Norm(camera_side_vec);
    glm::vec4 normalized_foward = foward_vector / Matrices::Norm(foward_vector);

    if (is_pressing_W)
        position = position + 0.03f * normalized_foward;
    if (is_pressing_S)
        position = position - 0.03f * normalized_foward;
    if (is_pressing_A)
        position = position + 0.03f * normalized_side;
    if (is_pressing_D)
        position = position - 0.03f * normalized_side;

    glm::mat4 view = Matrices::CameraView(position, camera_view_vector, camera_up_vector);

    float nearplane = -0.1f;
    float farplane = -10.0f;
    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrices::Perspective(field_of_view, screen_ratio, nearplane, farplane);

    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}