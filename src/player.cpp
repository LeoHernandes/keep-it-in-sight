#include "player.h"
#include <iostream>

//////////////////////
// Auxiliar functions
//////////////////////
void Player::UpdatePlayerPosition()
{
    glm::vec4 foward_vector = free_camera->view_vector;
    foward_vector.y = 0.0f; // Disable movement in y axis

    glm::vec4 camera_side_vec = Matrices::CrossProduct(free_camera->up_vector, foward_vector);
    glm::vec4 normalized_side = camera_side_vec / Matrices::Norm(camera_side_vec);
    glm::vec4 normalized_foward = foward_vector / Matrices::Norm(foward_vector);

    if (_is_pressing_W_key)
        position = position + velocity * normalized_foward;
    if (_is_pressing_S_key)
        position = position - velocity * normalized_foward;
    if (_is_pressing_A_key)
        position = position + velocity * normalized_side;
    if (_is_pressing_D_key)
        position = position - velocity * normalized_side;
}

///////////////
// Constructor
///////////////
Player::Player(FreeCamera *free_camera, LookAtCamera *look_at_camera)
{
    this->camera_mode = CameraMode::Free;
    this->look_at_camera = look_at_camera;
    this->free_camera = free_camera;
    this->velocity = 0.002f;
    this->position = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);

    this->_is_left_mouse_button_pressed = false;
    this->_is_pressing_W_key = false;
    this->_is_pressing_A_key = false;
    this->_is_pressing_S_key = false;
    this->_is_pressing_D_key = false;
    this->_lastCursorPosX = 0.0;
    this->_lastCursorPosY = 0.0;
}

/////////////
// On Update
/////////////
void Player::OnUpdate(GLint view_uniform, GLint projection_uniform)
{
    switch (camera_mode)
    {
    case CameraMode::LookAt:
        look_at_camera->Update(view_uniform, projection_uniform, position);
        break;
    case CameraMode::Free:
        UpdatePlayerPosition();
        free_camera->Update(view_uniform, projection_uniform, position);
        break;
    default:
        break;
    }
}

/////////////
// Callbacks
/////////////
void Player::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
{
    // Close game
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Toggle text
    if (key == GLFW_KEY_H && action == GLFW_PRESS)
    {
        show_info_text = !show_info_text;
    }

    // Toggle camera mode
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        switch (camera_mode)
        {
        case CameraMode::LookAt:
            camera_mode = CameraMode::Free;
            break;
        case CameraMode::Free:
            camera_mode = CameraMode::LookAt;
            break;
        default:
            break;
        }
    }

    // Movement inputs
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        _is_pressing_W_key = true;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        _is_pressing_W_key = false;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        _is_pressing_S_key = true;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        _is_pressing_S_key = false;
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        _is_pressing_A_key = true;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        _is_pressing_A_key = false;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        _is_pressing_D_key = true;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        _is_pressing_D_key = false;
    }
}

void Player::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    float zoom_delta = 0.1f * yoffset;

    // Keep a minimum distance from look at point
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (look_at_camera->distance - zoom_delta >= verysmallnumber)
        look_at_camera->distance -= zoom_delta;
}

void Player::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &_lastCursorPosX, &_lastCursorPosY);
        _is_left_mouse_button_pressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        _is_left_mouse_button_pressed = false;
    }
}

void Player::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    float dx = xpos - _lastCursorPosX;
    float dy = ypos - _lastCursorPosY;

    float phimax = 3.141592f / 2;
    float phimin = -phimax;

    switch (camera_mode)
    {
    case CameraMode::LookAt:
        if (_is_left_mouse_button_pressed)
        {
            look_at_camera->view_angle_theta -= 0.01f * dx;
            look_at_camera->view_angle_phi += 0.01f * dy;

            if (look_at_camera->view_angle_phi > phimax)
                look_at_camera->view_angle_phi = phimax;

            if (look_at_camera->view_angle_phi < phimin)
                look_at_camera->view_angle_phi = phimin;

            _lastCursorPosX = xpos;
            _lastCursorPosY = ypos;
        }
        break;
    case CameraMode::Free:
        free_camera->view_angle_theta -= 0.001f * dx;
        free_camera->view_angle_phi -= 0.001f * dy;

        if (free_camera->view_angle_phi > phimax)
            free_camera->view_angle_phi = phimax;

        if (free_camera->view_angle_phi < phimin)
            free_camera->view_angle_phi = phimin;

        _lastCursorPosX = xpos;
        _lastCursorPosY = ypos;
        break;
    default:
        break;
    }
}

void Player::ScreenRatioCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    look_at_camera->screen_ratio = (float)width / height;
    free_camera->screen_ratio = (float)width / height;
}