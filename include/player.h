#ifndef _PLAYER_H
#define _PLAYER_H

#include "lookAtCamera.h"

// OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum struct CameraMode
{
    Free,
    LookAt,
};

class Player
{
private:
    CameraMode camera_mode;
    bool _isLeftMouseButtonPressed = false;
    bool _isRightMouseButtonPressed = false;
    double _lastCursorPosX, _lastCursorPosY;

public:
    bool show_info_text;
    LookAtCamera *look_at_camera;

    Player(LookAtCamera *look_at_camera);
    void RenderView(GLint view_uniform, GLint projection_uniform);
    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void ScreenRatioCallback(GLFWwindow *window, int width, int height);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
};

#endif // _PLAYER_H