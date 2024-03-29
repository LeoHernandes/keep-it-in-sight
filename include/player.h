#ifndef _PLAYER_H
#define _PLAYER_H

#include "lookAtCamera.h"
#include "freeCamera.h"
#include "matrices.h"
#include "collisions.h"
#include "cubicBezier.h"
#include "audioManager.h"

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

    // player movement
    glm::vec4 velocity_vec;
    const float MAX_WALK_VELOCITY = 5.0f;
    const float MAX_RUN_VELOCITY = 10.0f;
    const float WALK_ACCELERATION = 50.0f;
    const float RUN_ACCELERATION = 60.0f;
    const float FRICTION_FACTOR = 100.0f;

    // player stamina
    float stamina = 0.0f;
    bool has_stamina = true;
    float time_without_run = 0.0f;
    const float TOTAL_STAMINA = 100.0f;
    const float STAMINA_RECOVERY_RATE = 10.0f;
    const float STAMINA_LOSS_RATE = 30.0f;
    const float TIME_TO_RUN_AGAIN_AFTER_USE_ALL_STAMINA = 3.0f;

    // player camera
    float time_without_flash = 0.0f;
    const float TIME_TO_USE_FLASH_CAMERA = 3.0f;
    const float MAX_DISTANCE_FLASH_CAMERA = 10.0f;

    CubicBezier *cubic_bezier_head_movement;
    glm::vec4 head_movement;
    const float DELTA_HEAD_MOVEMENT_POSITION = 0.15f;
    const float ANIMATION_TIME_HEAD_MOVEMENT = 0.2f;

    bool _is_left_mouse_button_pressed;
    bool _is_pressing_W_key;
    bool _is_pressing_A_key;
    bool _is_pressing_S_key;
    bool _is_pressing_D_key;
    bool _is_pressing_SHIFT_key;
    bool _is_pressing_F_key;
    double _lastCursorPosX, _lastCursorPosY;

    glm::vec4 GetPlayerAccelerationVector();
    void UpdatePlayerVelocityVector(float deltaTime, glm::vec4 acceleration_vec);
    void UpdatePlayerPosition(float deltaTime);
    void UpdatePlayerCameraEntity(float deltaTime);
    float GetDeltaRunVelocity();
    void LossStamina(float deltaTime);
    void RecoveryStamina(float deltaTime);

public:
    bool show_info_text;
    bool _is_pressing_E_key;
    LookAtCamera *look_at_camera;
    FreeCamera *free_camera;
    HitBox *monster_hitbox;
    glm::vec4 position;
    int collected_coins;
    bool hit_monster_with_flash_camera;

    Player();
    void AddFreeCamera(FreeCamera *camera);
    void AddLookAtCamera(LookAtCamera *camera);
    void AddMonsterHitbox(HitBox *monster_hitbox);
    void OnUpdate(float deltaTime);
    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos);
    void ScreenRatioCallback(GLFWwindow *window, int width, int height);
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
};

#endif // _PLAYER_H