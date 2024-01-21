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
    texture_loader.LoadTextureImage("../../data/tc-earth_daymap_surface.jpg");
    GpuProgramController gpu_controller(gpu_program_id);

    FreeCamera free_camera((float)WINDOW_WIDTH / WINDOW_HEIGHT, &gpu_controller);
    LookAtCamera look_at_camera((float)WINDOW_WIDTH / WINDOW_HEIGHT, &gpu_controller);
    player.AddFreeCamera(&free_camera);
    player.AddLookAtCamera(&look_at_camera);

    TextRendering_Init();

    Object bunny("../../data/bunny.obj");

    Scene scene;
    StaticEntity bunnymodel("bunny", &gpu_controller, Matrices::Translate(5.0f, 1.0f, 0.0f), &bunny);
    scene.AddEntity(&bunnymodel);
    StaticEntity bunnymodel2("bunny2", &gpu_controller, Matrices::Translate(-3.0f, 1.0f, 2.0f), &bunny);
    scene.AddEntity(&bunnymodel2);

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

        // Update camera projection matrix
        player.OnUpdate(deltaTime);
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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}