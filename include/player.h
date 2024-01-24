#ifndef _PLAYER_H
#define _PLAYER_H

#include "lookAtCamera.h"
#include "freeCamera.h"
#include "matrices.h"
#include "collisions.h"

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

    glm::vec4 velocity_vec;
    const float MAX_WALK_VELOCITY = 5.0f;
    const float MAX_RUN_VELOCITY = 10.0f;
    const float WALK_ACCELERATION = 35.0f;
    const float RUN_ACCELERATION = 50.0f;
    const float FRICTION_FACTOR = 20.0f;

    bool _is_left_mouse_button_pressed;
    bool _is_pressing_W_key;
    bool _is_pressing_A_key;
    bool _is_pressing_S_key;
    bool _is_pressing_D_key;
    bool _is_pressing_SHIFT_key = false;
    double _lastCursorPosX, _lastCursorPosY;

    glm::vec4 GetPlayerAccelerationVector();
    void UpdatePlayerVelocityVector(float deltaTime, glm::vec4 acceleration_vec);
    void UpdatePlayerPosition(float deltaTime);

public:
    bool show_info_text;
    LookAtCamera *look_at_camera;
    FreeCamera *free_camera;
    glm::vec4 position;

    Player();
    void AddFreeCamera(FreeCamera *camera);
    void AddLookAtCamera(LookAtCamera *camera);
    void OnUpdate(float deltaTime);
    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void ScreenRatioCallback(GLFWwindow *window, int width, int height);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
};

#endif // _PLAYER_H