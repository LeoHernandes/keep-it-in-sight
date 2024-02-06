// Standard libs
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <sstream>
#include <stack>
#include <string>
// OpenGL libs
#include <glad/glad.h>
// Window manager
#include <GLFW/glfw3.h>
// GLM: vectors and matrices
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
// Custom headers
#include "gpuProgramController.h"
#include "matrices.h"
#include "staticEntity.h"
#include "player.h"
#include "scene.h"
#include "shaders.h"
#include "textrendering.h"
#include "utils.h"
#include "textureLoader.h"
#include "skybox.h"
#include "door.h"
#include "coin.h"
#include "camera.h"
#include "monster.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

GLFWwindow *InitializeAppWindow(Player *player);
void SetupOpenGl();

int main()
{
    TextureLoader texture_loader;
    Player player;
    GLFWwindow *window = InitializeAppWindow(&player);

    SetupOpenGl();
    PrintGpuInfo();

    GLuint gpu_program_id = LoadShadersFromFiles();
    texture_loader.LoadTextureImage("../../data/skybox_fake_clouds.jpg");
    texture_loader.LoadTextureImage("../../data/tc-earth_daymap_surface.jpg");
    texture_loader.LoadTextureImage("../../data/10124_SLR_Camera_V1_Diffuse.jpg");
    texture_loader.LoadTextureImage("../../data/10057_wooden_door_v1_diffuse.jpg");
    texture_loader.LoadTextureImage("../../data/wall_backrooms.jpg");
    texture_loader.LoadTextureImage("../../data/floor_tile.jpg");
    texture_loader.LoadTextureImage("../../data/rusty_metal.jpg");

    GpuProgramController gpu_controller(gpu_program_id);

    FreeCamera free_camera((float)WINDOW_WIDTH / WINDOW_HEIGHT, &gpu_controller);
    LookAtCamera look_at_camera((float)WINDOW_WIDTH / WINDOW_HEIGHT, &gpu_controller);
    player.AddFreeCamera(&free_camera);
    player.AddLookAtCamera(&look_at_camera);

    TextRendering_Init();

    Object bunny("../../data/bunny.obj");
    Object sphere("../../data/sphere.obj");
    Object door("../../data/10057_wooden_door_v3_iterations-2.obj");
    Object camera("../../data/10124_SLR_Camera_SG_V1_Iteration2.obj");
    Object plane("../../data/plane.obj");
    Object cube("../../data/cube.obj");

    Scene scene;

    Door doormodel1("door1", &gpu_controller, Matrices::Identity(), &door, &player);
    doormodel1.CreateHitBox();
    doormodel1.SetScale(0.01f, 0.01f, 0.01f);
    doormodel1.SetPosition(0.0f, 0.0f, 5.0f);
    doormodel1.SetRotation(-3.141592 / 2, 0.0f, 0.0f);
    doormodel1.UpdateModel();
    doormodel1.UpdateCollision();

    Coin coinmodel1("coin1", &gpu_controller, Matrices::Identity(), &sphere, &player, &scene, glm::vec3(-3.0f, 0.5f, 7.0f));
    Coin coinmodel2("coin2", &gpu_controller, Matrices::Identity(), &sphere, &player, &scene, glm::vec3(0.0f, 0.5f, 7.0f));
    Coin coinmodel3("coin3", &gpu_controller, Matrices::Identity(), &sphere, &player, &scene, glm::vec3(3.0f, 0.5f, 7.0f));
    Coin coinmodel4("coin4", &gpu_controller, Matrices::Identity(), &sphere, &player, &scene, glm::vec3(6.0f, 0.5f, 7.0f));

    Camera cameramodel1("camera1", &gpu_controller, Matrices::Identity(), &camera, &player);
    cameramodel1.SetScale(0.002f, 0.002f, 0.002f);
    cameramodel1.SetPosition(-10.0f, 1.0f, 2.0f);
    cameramodel1.UpdateModel();

    StaticEntity planemodel1("plane1", &gpu_controller, Matrices::Identity(), &plane, 5, LightInterpolationType::NO_LIGHT);
    planemodel1.SetScale(10.0f, 1.0f, 10.0f);
    planemodel1.UpdateModel();

    StaticEntity cubemodel1("cube1", &gpu_controller, Matrices::Identity(), &cube, 4);
    cubemodel1.CreateHitBox();
    cubemodel1.SetScale(10.0f, 1.5f, 1.0f);
    cubemodel1.SetPosition(0.0f, 1.5f, 10.0f);
    cubemodel1.UpdateModel();
    cubemodel1.UpdateCollision();

    StaticEntity cubemodel2("cube2", &gpu_controller, Matrices::Identity(), &cube, 4);
    cubemodel2.CreateHitBox();
    cubemodel2.SetRotation(0.0f, -3.141592, 0.0f); // ADICIONADO PARA ARRUMAR TEXTURA
    cubemodel2.SetScale(10.0f, 1.5f, 1.0f);
    cubemodel2.SetPosition(0.0f, 1.5f, -10.0f);
    cubemodel2.UpdateModel();
    cubemodel2.UpdateCollision();

    StaticEntity cubemodel3("cube3", &gpu_controller, Matrices::Identity(), &cube, 4);
    cubemodel3.CreateHitBox();
    cubemodel3.SetScale(1.0f, 1.5f, 10.0f);
    cubemodel3.SetRotation(0.0f, 3 * -3.141592 / 2, 0.0f); // MODIFICADO PARA ARRUMAR TEXTURA
    cubemodel3.SetPosition(10.0f, 1.5f, 0.0f);
    cubemodel3.UpdateModel();
    cubemodel3.UpdateCollision();

    StaticEntity cubemodel4("cube4", &gpu_controller, Matrices::Identity(), &cube, 4);
    cubemodel4.CreateHitBox();
    cubemodel4.SetScale(1.0f, 1.5f, 10.0f);
    cubemodel4.SetRotation(0.0f, -3.141592 / 2, 0.0f);
    cubemodel4.SetPosition(-10.0f, 1.5f, 0.0f);
    cubemodel4.UpdateModel();
    cubemodel4.UpdateCollision();

    Monster monstermodel("monster", &gpu_controller, Matrices::Identity(), &bunny, &player);
    // monstermodel.CreateHitBox();
    monstermodel.SetPosition(10.0f, 1.0f, 10.0f);
    monstermodel.UpdateModel();
    // monstermodel.UpdateCollision();
    // player.AddMonsterHitbox(monstermodel.hit_box);

    Skybox skybox("skybox", &gpu_controller, &sphere, &player);

    scene.AddSkybox(&skybox);

    scene.AddEntity(&doormodel1);

    scene.AddEntity(&coinmodel1);
    scene.AddEntity(&coinmodel2);
    scene.AddEntity(&coinmodel3);
    scene.AddEntity(&coinmodel4);

    scene.AddEntity(&cameramodel1);

    scene.AddEntity(&planemodel1);

    scene.AddEntity(&cubemodel1);
    scene.AddEntity(&cubemodel2);
    scene.AddEntity(&cubemodel3);
    scene.AddEntity(&cubemodel4);

    scene.AddEntity(&monstermodel);

    float prevTime = glfwGetTime();
    float currentTime = 0.0f;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // Reset screen pixels data
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Specifies the handle of the program object whose executables
        // are to be used in GPU as part of current rendering state
        glUseProgram(gpu_program_id);

        currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        // printf("collected coins: %d\n", player.collected_coins);

        // Update camera projection matrix
        player.OnUpdate(deltaTime);
        scene.Update(deltaTime);
        scene.Render();

        if (player.show_info_text)
            TextRendering_ShowFramesPerSecond(window);

        // Double buffering swap to avoid screen tearing
        glfwSwapBuffers(window);

        // User interaction poll
        glfwPollEvents();
    }

    // Destroys and frees all GLFW resources
    glfwTerminate();
    return 0;
}

GLFWwindow *InitializeAppWindow(Player *player)
{
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback([](int error, const char *description)
                         { fprintf(stderr, "ERROR: GLFW: %s\n", description); });

    // OpenGL version >= 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Require OpenGL core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // App window creation
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Keep It In Sight", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    // Locks mouse inside window
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Player inputs callbacks
    glfwSetWindowUserPointer(window, player);

    auto keyCallBack = [](GLFWwindow *window, int key, int scancode, int action, int mod)
    {
        Player *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
        player->KeyCallback(window, key, scancode, action, mod);
    };
    glfwSetKeyCallback(window, keyCallBack);

    auto scrollCallBack = [](GLFWwindow *window, double xoffset, double yoffset)
    {
        Player *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
        player->ScrollCallback(window, xoffset, yoffset);
    };
    glfwSetScrollCallback(window, scrollCallBack);

    auto mouseButtonCallBack = [](GLFWwindow *window, int button, int action, int mods)
    {
        Player *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
        player->MouseButtonCallback(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(window, mouseButtonCallBack);

    auto cursorPosCallBack = [](GLFWwindow *window, double xpos, double ypos)
    {
        Player *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
        player->CursorPosCallback(window, xpos, ypos);
    };
    glfwSetCursorPosCallback(window, cursorPosCallBack);

    // Window resize callback
    auto screenRatioCallback = [](GLFWwindow *window, int width, int height)
    {
        Player *player = static_cast<Player *>(glfwGetWindowUserPointer(window));
        player->ScreenRatioCallback(window, width, height);
    };
    glfwSetFramebufferSizeCallback(window, screenRatioCallback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    return window;
}

void SetupOpenGl()
{
    // Load OpenGL 3.3 functions
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);

    // Enable Backface Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}