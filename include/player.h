#ifndef _PLAYER_H
#define _PLAYER_H

#include "lookAtCamera.h"
#include "freeCamera.h"
#include "matrices.h"

// OpenGl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// Matrices
#include <glm/vec4.hpp>

enum struct CameraMode
{
    Free,
    LookAt,
};

class Player
{
private:
    CameraMode camera_mode;
    float velocity;

    bool _is_left_mouse_button_pressed;
    bool _is_pressing_W_key;
    bool _is_pressing_A_key;
    bool _is_pressing_S_key;
    bool _is_pressing_D_key;
    double _lastCursorPosX, _lastCursorPosY;

    void UpdatePlayerPosition();

public:
    bool show_info_text;
    LookAtCamera *look_at_camera;
    FreeCamera *free_camera;
    glm::vec4 position;

    Player();
    void AddFreeCamera(FreeCamera *camera);
    void AddLookAtCamera(LookAtCamera *camera);
    void OnUpdate();
    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void ScreenRatioCallback(GLFWwindow *window, int width, int height);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
};

#endif // _PLAYER_H