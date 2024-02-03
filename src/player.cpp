#include "player.h"

//////////////////////
// Auxiliar functions
//////////////////////
float Player::GetDeltaRunVelocity()
{
    // How much percent is the current velocity between MAX_WALK_VELOCITY and MAX_RUN_VELOCITY
    float delta_run_velocity = (Matrices::Norm(velocity_vec) - MAX_WALK_VELOCITY) / (MAX_RUN_VELOCITY - MAX_WALK_VELOCITY);
    if (delta_run_velocity < 0)
        return 0;

    return delta_run_velocity;
}

glm::vec4 Player::GetPlayerAccelerationVector()
{
    glm::vec4 foward_vector = free_camera->view_vector;
    foward_vector.y = 0.0f; // Disable movement in y axis
    glm::vec4 camera_side_vec = Matrices::CrossProduct(free_camera->up_vector, foward_vector);

    // Define acceleration vector
    glm::vec4 acceleration_vec = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if (_is_pressing_W_key)
        acceleration_vec = acceleration_vec + foward_vector;
    if (_is_pressing_S_key)
        acceleration_vec = acceleration_vec - foward_vector;
    if (_is_pressing_A_key)
        acceleration_vec = acceleration_vec + camera_side_vec;
    if (_is_pressing_D_key)
        acceleration_vec = acceleration_vec - camera_side_vec;

    return acceleration_vec;
}

void Player::LossStamina(float deltaTime)
{
    float new_stamina = this->stamina - STAMINA_LOSS_RATE * deltaTime;

    if (time_without_run >= TIME_TO_RUN_AGAIN_AFTER_USE_ALL_STAMINA)
        this->has_stamina = true;

    if (new_stamina >= 0.0f)
    {
        this->stamina = new_stamina;
    }
    else
    {
        this->has_stamina = false;
        time_without_run = 0.0f;
    }
}

void Player::RecoveryStamina(float deltaTime)
{
    time_without_run += deltaTime;
    float new_stamina = this->stamina + STAMINA_RECOVERY_RATE * deltaTime;

    if (time_without_run >= TIME_TO_RUN_AGAIN_AFTER_USE_ALL_STAMINA)
        this->has_stamina = true;

    if (new_stamina <= TOTAL_STAMINA)
        this->stamina = new_stamina;
}

void Player::UpdatePlayerVelocityVector(float deltaTime, glm::vec4 acceleration_vec)
{
    if (!Matrices::IsVectorNull(acceleration_vec))
    {
        glm::vec4 normalized_acceleration_vec = Matrices::Normalize(acceleration_vec);
        if (_is_pressing_SHIFT_key && has_stamina)
        {
            glm::vec4 new_velocity_vec = this->velocity_vec + normalized_acceleration_vec * deltaTime * RUN_ACCELERATION;
            float current_velocity = Matrices::Norm(new_velocity_vec);

            if (current_velocity <= MAX_RUN_VELOCITY)
                this->velocity_vec = new_velocity_vec;

            LossStamina(deltaTime);

            this->cubic_bezier_head_movement->Update(deltaTime);
            this->head_movement = cubic_bezier_head_movement->GetPoint();
        }
        else
        {
            glm::vec4 new_velocity_vec = this->velocity_vec + normalized_acceleration_vec * deltaTime * WALK_ACCELERATION;
            float current_velocity = Matrices::Norm(new_velocity_vec);

            if (current_velocity <= MAX_WALK_VELOCITY)
                this->velocity_vec = new_velocity_vec;
            else
            {
                glm::vec4 normalized_velocity_vec = Matrices::Normalize(velocity_vec);
                glm::vec4 friction_vec = -normalized_velocity_vec * deltaTime * FRICTION_FACTOR;
                this->velocity_vec += friction_vec;
            }

            RecoveryStamina(deltaTime);
        }
    }
    else
    {
        // If there is no acceleration, apply friction on velocity vec
        if (!Matrices::IsVectorNull(this->velocity_vec))
        {
            glm::vec4 normalized_velocity_vec = Matrices::Normalize(velocity_vec);
            glm::vec4 friction_vec = -normalized_velocity_vec * deltaTime * FRICTION_FACTOR;

            if (Matrices::Norm(velocity_vec) <= Matrices::Norm(friction_vec))
                this->velocity_vec = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            else
                this->velocity_vec += friction_vec;
        }

        RecoveryStamina(deltaTime);
    }

    //printf("stamina: %f, time_without_run: %f\n", this->stamina, time_without_run);
}

void Player::UpdatePlayerPosition(float deltaTime)
{
    glm::vec4 acceleration_vec = GetPlayerAccelerationVector();
    UpdatePlayerVelocityVector(deltaTime, acceleration_vec);

    //printf("%d\n", Collisions::PointBoxTest(this->position + velocity_vec * deltaTime));

    // Update player position
    if (!Collisions::PointBoxTest(this->position + velocity_vec * deltaTime))
        this->position += velocity_vec * deltaTime;
}

///////////////
// Constructor
///////////////
Player::Player()
{
    this->camera_mode = CameraMode::Free;
    this->velocity_vec = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    this->position = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);
    this->show_info_text = false;

    this->_is_left_mouse_button_pressed = false;
    this->_is_pressing_W_key = false;
    this->_is_pressing_A_key = false;
    this->_is_pressing_S_key = false;
    this->_is_pressing_D_key = false;
    this->_lastCursorPosX = 0.0;
    this->_lastCursorPosY = 0.0;
    
    this->cubic_bezier_head_movement = new CubicBezier(ANIMATION_TIME_HEAD_MOVEMENT,
                                                       glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
                                                       glm::vec4(0.0f, DELTA_HEAD_MOVEMENT_POSITION / 3, 0.0f, 0.0f),
                                                       glm::vec4(0.0f, DELTA_HEAD_MOVEMENT_POSITION / 3 * 2, 0.0f, 0.0f),
                                                       glm::vec4(0.0f, DELTA_HEAD_MOVEMENT_POSITION, 0.0f, 0.0f));
}

/////////////
// Cameras
/////////////
void Player::AddFreeCamera(FreeCamera *camera)
{
    this->free_camera = camera;
}

void Player::AddLookAtCamera(LookAtCamera *camera)
{
    this->look_at_camera = camera;
}

/////////////
// On Update
/////////////
void Player::OnUpdate(float deltaTime)
{
    switch (camera_mode)
    {
    case CameraMode::LookAt:
        look_at_camera->Update(position);
        break;
    case CameraMode::Free:
        UpdatePlayerPosition(deltaTime);
        free_camera->Update(position, GetDeltaRunVelocity(), this->head_movement);
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
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    {
        _is_pressing_SHIFT_key = true;
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    {
        _is_pressing_SHIFT_key = false;
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