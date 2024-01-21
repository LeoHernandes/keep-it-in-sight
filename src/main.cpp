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
#include "cubeEntity.h"
#include "cubeEntity2.h"
#include "gpuProgramController.h"
#include "matrices.h"
#include "objEntity.h"
#include "player.h"
#include "scene.h"
#include "shaders.h"
#include "textrendering.h"
#include "utils.h"

#include "stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Número de texturas carregadas pela função LoadTextureImage()
GLuint g_NumLoadedTextures = 0;

GLFWwindow *InitializeAppWindow(Player *player);
void SetupOpenGl();
void LoadTextureImage(const char *filename);

int main()
{
    Player player;
    GLFWwindow *window = InitializeAppWindow(&player);

    SetupOpenGl();
    PrintGpuInfo();

    GLuint gpu_program_id = LoadShadersFromFiles();
    LoadTextureImage("../../data/tc-earth_daymap_surface.jpg");
    GpuProgramController gpu_controller(gpu_program_id);

    FreeCamera free_camera((float)WINDOW_WIDTH / WINDOW_HEIGHT, &gpu_controller);
    LookAtCamera look_at_camera((float)WINDOW_WIDTH / WINDOW_HEIGHT, &gpu_controller);
    player.AddFreeCamera(&free_camera);
    player.AddLookAtCamera(&look_at_camera);

    TextRendering_Init();

    Scene scene;
    ObjEntity bunnymodel("../../data/bunny.obj", "bunny", &gpu_controller);
    scene.AddEntity(&bunnymodel);

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

// Função que carrega uma imagem para ser utilizada como textura
void LoadTextureImage(const char *filename)
{
    printf("Carregando imagem \"%s\"... ", filename);

    // Primeiro fazemos a leitura da imagem do disco
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);

    if (data == NULL)
    {
        fprintf(stderr, "ERROR: Cannot open image file \"%s\".\n", filename);
        std::exit(EXIT_FAILURE);
    }

    printf("OK (%dx%d).\n", width, height);

    // Agora criamos objetos na GPU com OpenGL para armazenar a textura
    GLuint texture_id;
    GLuint sampler_id;
    glGenTextures(1, &texture_id);
    glGenSamplers(1, &sampler_id);

    // Veja slides 95-96 do documento Aula_20_Mapeamento_de_Texturas.pdf
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Parâmetros de amostragem da textura.
    glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Agora enviamos a imagem lida do disco para a GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    GLuint textureunit = g_NumLoadedTextures;
    glActiveTexture(GL_TEXTURE0 + textureunit);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindSampler(textureunit, sampler_id);

    stbi_image_free(data);

    g_NumLoadedTextures += 1;
}