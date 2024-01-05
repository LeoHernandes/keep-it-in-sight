#include "lookAtCamera.h"
#include "matrices.h"

LookAtCamera::LookAtCamera(float screen_ratio)
{
    this->view_angle_theta = 0.0f;
    this->view_angle_phi = 0.0f;
    this->distance = 3.5f;
    this->screen_ratio = screen_ratio;
}

void LookAtCamera::Update(GLint view_uniform, GLint projection_uniform)
{
    // Look at camera position
    float r = distance;
    float y = r * sin(view_angle_phi);
    float z = r * cos(view_angle_phi) * cos(view_angle_theta);
    float x = r * cos(view_angle_phi) * sin(view_angle_theta);

    glm::vec4 camera_position_c = glm::vec4(x, y, z, 1.0f);
    glm::vec4 camera_lookat_l = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 camera_view_vector = camera_lookat_l - camera_position_c;
    glm::vec4 camera_up_vector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

    float nearplane = -0.1f;
    float farplane = -10.0f;
    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrix_Perspective(field_of_view, screen_ratio, nearplane, farplane);

    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
}

void LookAtCamera::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    float zoom_delta = 0.1f * yoffset;

    // Keep a minimum distance from look at point
    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (this->distance - zoom_delta >= verysmallnumber)
        this->distance -= zoom_delta;
}

void LookAtCamera::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
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
void LookAtCamera::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (_isLeftMouseButtonPressed)
    {
        float dx = xpos - _lastCursorPosX;
        float dy = ypos - _lastCursorPosY;

        view_angle_theta -= 0.01f * dx;
        view_angle_phi += 0.01f * dy;

        float phimax = 3.141592f / 2;
        float phimin = -phimax;

        if (view_angle_phi > phimax)
            view_angle_phi = phimax;

        if (view_angle_phi < phimin)
            view_angle_phi = phimin;

        _lastCursorPosX = xpos;
        _lastCursorPosY = ypos;
    }

    if (_isRightMouseButtonPressed)
    {
        _lastCursorPosX = xpos;
        _lastCursorPosY = ypos;
    }
}

void LookAtCamera::ScreenRatioCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_ratio = (float)width / height;
}