#include "player.h"

Player::Player(LookAtCamera *look_at_camera)
{
    this->camera_mode = CameraMode::LookAt;
    this->look_at_camera = look_at_camera;
}

void Player::RenderView(GLint view_uniform, GLint projection_uniform)
{
    switch (camera_mode)
    {
    case CameraMode::LookAt:
        look_at_camera->Update(view_uniform, projection_uniform);
        break;
    case CameraMode::Free:
        break;
    default:
        break;
    }
}

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
        _isLeftMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        _isLeftMouseButtonPressed = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &_lastCursorPosX, &_lastCursorPosY);
        _isRightMouseButtonPressed = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        _isRightMouseButtonPressed = false;
    }
}
void Player::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (_isLeftMouseButtonPressed)
    {
        float dx = xpos - _lastCursorPosX;
        float dy = ypos - _lastCursorPosY;

        look_at_camera->view_angle_theta -= 0.01f * dx;
        look_at_camera->view_angle_phi += 0.01f * dy;

        float phimax = 3.141592f / 2;
        float phimin = -phimax;

        if (look_at_camera->view_angle_phi > phimax)
            look_at_camera->view_angle_phi = phimax;

        if (look_at_camera->view_angle_phi < phimin)
            look_at_camera->view_angle_phi = phimin;

        _lastCursorPosX = xpos;
        _lastCursorPosY = ypos;
    }

    if (_isRightMouseButtonPressed)
    {
        _lastCursorPosX = xpos;
        _lastCursorPosY = ypos;
    }
}

void Player::ScreenRatioCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    look_at_camera->screen_ratio = (float)width / height;
}