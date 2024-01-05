// Standard libs
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <stack>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
// OpenGL libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM: vectors and matrices
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
// Custom headers
#include "utils.h"
#include "textrendering.h"
#include "shaders.h"
#include "input.h"
#include "lookAtCamera.h"
#include <functional>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

void DrawCube(GLint render_as_black_uniform);
GLuint BuildTriangles();
GLFWwindow *InitializeAppWindow(LookAtCamera *camera);
void SetupOpenGl();

// Temporary workaround for cross lib includes problems
// In the future, this matrices functions will be called only inside other classes, not directly in main
glm::mat4 Matrix_Identity();
glm::mat4 Matrix_Translate(float tx, float ty, float tz);
glm::mat4 Matrix_Camera_View(glm::vec4 position_c, glm::vec4 view_vector, glm::vec4 up_vector);
glm::mat4 Matrix_Perspective(float field_of_view, float aspect, float n, float f);

// TODO: Refactor this to use our Entity and Scene classes
struct SceneObject
{
    const char *name;
    void *first_index;
    int num_indices;
    GLenum rendering_mode;
};
std::map<const char *, SceneObject> g_VirtualScene;

extern float g_CubePositionX;
extern float g_CubePositionY;

extern bool g_ShowInfoText;

int main()
{
    LookAtCamera camera((float)WINDOW_WIDTH / WINDOW_HEIGHT);

    // App window creation
    GLFWwindow *window = InitializeAppWindow(&camera);

    SetupOpenGl();

    PrintGpuInfo();

    GLuint gpu_program_id = LoadShadersFromFiles();

    TextRendering_Init();

    GLuint vertex_array_object_id = BuildTriangles();
    // Get variables addresses from Vertex Shader file.
    GLint model_uniform = glGetUniformLocation(gpu_program_id, "model");
    GLint render_as_black_uniform = glGetUniformLocation(gpu_program_id, "render_as_black");
    GLint view_uniform = glGetUniformLocation(gpu_program_id, "view");
    GLint projection_uniform = glGetUniformLocation(gpu_program_id, "projection");

    while (!glfwWindowShouldClose(window))
    {
        // Reset screen pixels data
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Specifies the handle of the program object whose executables
        // are to be used in GPU as part of current rendering state
        glUseProgram(gpu_program_id);

        // UpdateCamera(view_uniform, projection_uniform);
        camera.Update(view_uniform, projection_uniform);

        // ************ Draw cube ************
        glBindVertexArray(vertex_array_object_id);
        glm::mat4 model = Matrix_Identity() * Matrix_Translate(g_CubePositionX - 1.0f, g_CubePositionY + 1.0f, 0.0f);
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        DrawCube(render_as_black_uniform);

        model = Matrix_Identity();
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));
        glLineWidth(10.0f);
        glUniform1i(render_as_black_uniform, false);
        glDrawElements(
            g_VirtualScene["axes"].rendering_mode,
            g_VirtualScene["axes"].num_indices,
            GL_UNSIGNED_INT,
            (void *)g_VirtualScene["axes"].first_index);
        glBindVertexArray(0);
        // ************************

        if (g_ShowInfoText)
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

// Função que desenha um cubo com arestas em preto, definido dentro da função BuildTriangles().
void DrawCube(GLint render_as_black_uniform)
{
    // Informamos para a placa de vídeo (GPU) que a variável booleana
    // "render_as_black" deve ser colocada como "false". Veja o arquivo
    // "shader_vertex.glsl".
    glUniform1i(render_as_black_uniform, false);

    // Pedimos para a GPU rasterizar os vértices do cubo apontados pelo
    // VAO como triângulos, formando as faces do cubo. Esta
    // renderização irá executar o Vertex Shader definido no arquivo
    // "shader_vertex.glsl", e o mesmo irá utilizar as matrizes
    // "model", "view" e "projection" definidas acima e já enviadas
    // para a placa de vídeo (GPU).
    //
    // Veja a definição de g_VirtualScene["cube_faces"] dentro da
    // função BuildTriangles(), e veja a documentação da função
    // glDrawElements() em http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene["cube_faces"].rendering_mode, // Veja slides 182-188 do documento Aula_04_Modelagem_Geometrica_3D.pdf
        g_VirtualScene["cube_faces"].num_indices,    //
        GL_UNSIGNED_INT,
        (void *)g_VirtualScene["cube_faces"].first_index);

    // Pedimos para OpenGL desenhar linhas com largura de 4 pixels.
    glLineWidth(4.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene["axes"] dentro da função BuildTriangles(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    //
    // Importante: estes eixos serão desenhamos com a matriz "model"
    // definida acima, e portanto sofrerão as mesmas transformações
    // geométricas que o cubo. Isto é, estes eixos estarão
    // representando o sistema de coordenadas do modelo (e não o global)!
    glDrawElements(
        g_VirtualScene["axes"].rendering_mode,
        g_VirtualScene["axes"].num_indices,
        GL_UNSIGNED_INT,
        (void *)g_VirtualScene["axes"].first_index);

    // Informamos para a placa de vídeo (GPU) que a variável booleana
    // "render_as_black" deve ser colocada como "true". Veja o arquivo
    // "shader_vertex.glsl".
    glUniform1i(render_as_black_uniform, true);

    // Pedimos para a GPU rasterizar os vértices do cubo apontados pelo
    // VAO como linhas, formando as arestas pretas do cubo. Veja a
    // definição de g_VirtualScene["cube_edges"] dentro da função
    // BuildTriangles(), e veja a documentação da função
    // glDrawElements() em http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        g_VirtualScene["cube_edges"].rendering_mode,
        g_VirtualScene["cube_edges"].num_indices,
        GL_UNSIGNED_INT,
        (void *)g_VirtualScene["cube_edges"].first_index);
}

// Constrói triângulos para futura renderização
GLuint BuildTriangles()
{
    // Primeiro, definimos os atributos de cada vértice.

    // A posição de cada vértice é definida por coeficientes em um sistema de
    // coordenadas local de cada modelo geométrico. Note o uso de coordenadas
    // homogêneas.  Veja as seguintes referências:
    //
    //  - slides 35-48 do documento Aula_08_Sistemas_de_Coordenadas.pdf;
    //  - slides 184-190 do documento Aula_08_Sistemas_de_Coordenadas.pdf;
    //
    // Este vetor "model_coefficients" define a GEOMETRIA (veja slides 103-110 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    //
    GLfloat model_coefficients[] = {
        // Vértices de um cubo
        //    X      Y     Z     W
        -0.5f, 0.0f, 0.5f, 1.0f,   // posição do vértice 0
        -0.5f, -1.0f, 0.5f, 1.0f,  // posição do vértice 1
        0.5f, -1.0f, 0.5f, 1.0f,   // posição do vértice 2
        0.5f, 0.0f, 0.5f, 1.0f,    // posição do vértice 3
        -0.5f, 0.0f, -0.5f, 1.0f,  // posição do vértice 4
        -0.5f, -1.0f, -0.5f, 1.0f, // posição do vértice 5
        0.5f, -1.0f, -0.5f, 1.0f,  // posição do vértice 6
        0.5f, 0.0f, -0.5f, 1.0f,   // posição do vértice 7
                                   // Vértices para desenhar o eixo X
                                   //    X      Y     Z     W
        0.0f, 0.0f, 0.0f, 1.0f,    // posição do vértice 8
        1.0f, 0.0f, 0.0f, 1.0f,    // posição do vértice 9
                                   // Vértices para desenhar o eixo Y
                                   //    X      Y     Z     W
        0.0f, 0.0f, 0.0f, 1.0f,    // posição do vértice 10
        0.0f, 1.0f, 0.0f, 1.0f,    // posição do vértice 11
                                   // Vértices para desenhar o eixo Z
                                   //    X      Y     Z     W
        0.0f, 0.0f, 0.0f, 1.0f,    // posição do vértice 12
        0.0f, 0.0f, 1.0f, 1.0f,    // posição do vértice 13
    };

    // Criamos o identificador (ID) de um Vertex Buffer Object (VBO).  Um VBO é
    // um buffer de memória que irá conter os valores de um certo atributo de
    // um conjunto de vértices; por exemplo: posição, cor, normais, coordenadas
    // de textura.  Neste exemplo utilizaremos vários VBOs, um para cada tipo de atributo.
    // Agora criamos um VBO para armazenarmos um atributo: posição.
    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);

    // Criamos o identificador (ID) de um Vertex Array Object (VAO).  Um VAO
    // contém a definição de vários atributos de um certo conjunto de vértices;
    // isto é, um VAO irá conter ponteiros para vários VBOs.
    GLuint vertex_array_object_id;
    glGenVertexArrays(1, &vertex_array_object_id);

    // "Ligamos" o VAO ("bind"). Informamos que iremos atualizar o VAO cujo ID
    // está contido na variável "vertex_array_object_id".
    glBindVertexArray(vertex_array_object_id);

    // "Ligamos" o VBO ("bind"). Informamos que o VBO cujo ID está contido na
    // variável VBO_model_coefficients_id será modificado a seguir. A
    // constante "GL_ARRAY_BUFFER" informa que esse buffer é de fato um VBO, e
    // irá conter atributos de vértices.
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);

    // Alocamos memória para o VBO "ligado" acima. Como queremos armazenar
    // nesse VBO todos os valores contidos no array "model_coefficients", pedimos
    // para alocar um número de bytes exatamente igual ao tamanho ("size")
    // desse array. A constante "GL_STATIC_DRAW" dá uma dica para o driver da
    // GPU sobre como utilizaremos os dados do VBO. Neste caso, estamos dizendo
    // que não pretendemos alterar tais dados (são estáticos: "STATIC"), e
    // também dizemos que tais dados serão utilizados para renderizar ou
    // desenhar ("DRAW").  Pense que:
    //
    //            glBufferData()  ==  malloc() do C  ==  new do C++.
    //
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_coefficients), NULL, GL_STATIC_DRAW);

    // Finalmente, copiamos os valores do array model_coefficients para dentro do
    // VBO "ligado" acima.  Pense que:
    //
    //            glBufferSubData()  ==  memcpy() do C.
    //
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model_coefficients), model_coefficients);

    // Precisamos então informar um índice de "local" ("location"), o qual será
    // utilizado no shader "shader_vertex.glsl" para acessar os valores
    // armazenados no VBO "ligado" acima. Também, informamos a dimensão (número de
    // coeficientes) destes atributos. Como em nosso caso são pontos em coordenadas
    // homogêneas, temos quatro coeficientes por vértice (X,Y,Z,W). Isso define
    // um tipo de dado chamado de "vec4" em "shader_vertex.glsl": um vetor com
    // quatro coeficientes. Finalmente, informamos que os dados estão em ponto
    // flutuante com 32 bits (GL_FLOAT).
    // Esta função também informa que o VBO "ligado" acima em glBindBuffer()
    // está dentro do VAO "ligado" acima por glBindVertexArray().
    // Veja https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
    GLuint location = 0;            // "(location = 0)" em "shader_vertex.glsl"
    GLint number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);

    // "Ativamos" os atributos. Informamos que os atributos com índice de local
    // definido acima, na variável "location", deve ser utilizado durante o
    // rendering.
    glEnableVertexAttribArray(location);

    // "Desligamos" o VBO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Agora repetimos todos os passos acima para atribuir um novo atributo a
    // cada vértice: uma cor (veja slides 109-112 do documento Aula_03_Rendering_Pipeline_Grafico.pdf e slide 111 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    // Tal cor é definida como coeficientes RGBA: Red, Green, Blue, Alpha;
    // isto é: Vermelho, Verde, Azul, Alpha (valor de transparência).
    // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
    GLfloat color_coefficients[] = {
        // Cores dos vértices do cubo
        //  R     G     B     A
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 0
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 1
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 2
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 3
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 4
        1.0f, 0.5f, 0.0f, 1.0f, // cor do vértice 5
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 6
        0.0f, 0.5f, 1.0f, 1.0f, // cor do vértice 7
                                // Cores para desenhar o eixo X
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 8
        1.0f, 0.0f, 0.0f, 1.0f, // cor do vértice 9
                                // Cores para desenhar o eixo Y
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 10
        0.0f, 1.0f, 0.0f, 1.0f, // cor do vértice 11
                                // Cores para desenhar o eixo Z
        0.0f, 0.0f, 1.0f, 1.0f, // cor do vértice 12
        0.0f, 0.0f, 1.0f, 1.0f, // cor do vértice 13
    };
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1;             // "(location = 1)" em "shader_vertex.glsl"
    number_of_dimensions = 4; // vec4 em "shader_vertex.glsl"
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Vamos então definir polígonos utilizando os vértices do array
    // model_coefficients.
    //
    // Para referência sobre os modos de renderização, veja slides 182-188 do documento Aula_04_Modelagem_Geometrica_3D.pdf.
    //
    // Este vetor "indices" define a TOPOLOGIA (veja slides 103-110 do documento Aula_04_Modelagem_Geometrica_3D.pdf).
    //
    GLuint indices[] = {
        // Definimos os índices dos vértices que definem as FACES de um cubo
        // através de 12 triângulos que serão desenhados com o modo de renderização
        // GL_TRIANGLES.
        0, 1, 2, // triângulo 1
        7, 6, 5, // triângulo 2
        3, 2, 6, // triângulo 3
        4, 0, 3, // triângulo 4
        4, 5, 1, // triângulo 5
        1, 5, 6, // triângulo 6
        0, 2, 3, // triângulo 7
        7, 5, 4, // triângulo 8
        3, 6, 7, // triângulo 9
        4, 3, 7, // triângulo 10
        4, 1, 0, // triângulo 11
        1, 6, 2, // triângulo 12
                 // Definimos os índices dos vértices que definem as ARESTAS de um cubo
                 // através de 12 linhas que serão desenhadas com o modo de renderização
                 // GL_LINES.
        0, 1,    // linha 1
        1, 2,    // linha 2
        2, 3,    // linha 3
        3, 0,    // linha 4
        0, 4,    // linha 5
        4, 7,    // linha 6
        7, 6,    // linha 7
        6, 2,    // linha 8
        6, 5,    // linha 9
        5, 4,    // linha 10
        5, 1,    // linha 11
        7, 3,    // linha 12
                 // Definimos os índices dos vértices que definem as linhas dos eixos X, Y,
                 // Z, que serão desenhados com o modo GL_LINES.
        8, 9,    // linha 1
        10, 11,  // linha 2
        12, 13   // linha 3
    };

    // Criamos um primeiro objeto virtual (SceneObject) que se refere às faces
    // coloridas do cubo.
    SceneObject cube_faces;
    cube_faces.name = "Cubo (faces coloridas)";
    cube_faces.first_index = (void *)0;       // Primeiro índice está em indices[0]
    cube_faces.num_indices = 36;              // Último índice está em indices[35]; total de 36 índices.
    cube_faces.rendering_mode = GL_TRIANGLES; // Índices correspondem ao tipo de rasterização GL_TRIANGLES.

    // Adicionamos o objeto criado acima na nossa cena virtual (g_VirtualScene).
    g_VirtualScene["cube_faces"] = cube_faces;

    // Criamos um segundo objeto virtual (SceneObject) que se refere às arestas
    // pretas do cubo.
    SceneObject cube_edges;
    cube_edges.name = "Cubo (arestas pretas)";
    cube_edges.first_index = (void *)(36 * sizeof(GLuint)); // Primeiro índice está em indices[36]
    cube_edges.num_indices = 24;                            // Último índice está em indices[59]; total de 24 índices.
    cube_edges.rendering_mode = GL_LINES;                   // Índices correspondem ao tipo de rasterização GL_LINES.

    // Adicionamos o objeto criado acima na nossa cena virtual (g_VirtualScene).
    g_VirtualScene["cube_edges"] = cube_edges;

    // Criamos um terceiro objeto virtual (SceneObject) que se refere aos eixos XYZ.
    SceneObject axes;
    axes.name = "Eixos XYZ";
    axes.first_index = (void *)(60 * sizeof(GLuint)); // Primeiro índice está em indices[60]
    axes.num_indices = 6;                             // Último índice está em indices[65]; total de 6 índices.
    axes.rendering_mode = GL_LINES;                   // Índices correspondem ao tipo de rasterização GL_LINES.
    g_VirtualScene["axes"] = axes;

    // Criamos um buffer OpenGL para armazenar os índices acima
    GLuint indices_id;
    glGenBuffers(1, &indices_id);

    // "Ligamos" o buffer. Note que o tipo agora é GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

    // Alocamos memória para o buffer.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);

    // Copiamos os valores do array indices[] para dentro do buffer.
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);

    // NÃO faça a chamada abaixo! Diferente de um VBO (GL_ARRAY_BUFFER), um
    // array de índices (GL_ELEMENT_ARRAY_BUFFER) não pode ser "desligado",
    // caso contrário o VAO irá perder a informação sobre os índices.
    //
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // XXX Errado!
    //

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);

    // Retornamos o ID do VAO. Isso é tudo que será necessário para renderizar
    // os triângulos definidos acima. Veja a chamada glDrawElements() em main().
    return vertex_array_object_id;
}

GLFWwindow *InitializeAppWindow(LookAtCamera *camera)
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

    // Player inputs callbacks
    glfwSetKeyCallback(window, KeyCallback);

    // ISSO AQUI É BRUXARIA, DEPOIS EU TE EXPLICO
    glfwSetWindowUserPointer(window, camera);

    auto scrollCallBack = [](GLFWwindow *window, double xoffset, double yoffset)
    {
        LookAtCamera *camera = static_cast<LookAtCamera *>(glfwGetWindowUserPointer(window));
        assert(camera);
        camera->ScrollCallback(window, xoffset, yoffset);
    };
    glfwSetScrollCallback(window, scrollCallBack);

    auto mouseButtonCallBack = [](GLFWwindow *window, int button, int action, int mods)
    {
        LookAtCamera *camera = static_cast<LookAtCamera *>(glfwGetWindowUserPointer(window));
        assert(camera);
        camera->MouseButtonCallback(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(window, mouseButtonCallBack);

    auto cursorPosCallBack = [](GLFWwindow *window, double xpos, double ypos)
    {
        LookAtCamera *camera = static_cast<LookAtCamera *>(glfwGetWindowUserPointer(window));
        assert(camera);
        camera->CursorPosCallback(window, xpos, ypos);
    };
    glfwSetCursorPosCallback(window, cursorPosCallBack);

    // Window resize callback
    auto screenRatioCallback = [](GLFWwindow *window, int width, int height)
    {
        LookAtCamera *camera = static_cast<LookAtCamera *>(glfwGetWindowUserPointer(window));
        assert(camera);
        camera->ScreenRatioCallback(window, width, height);
    };
    glfwSetFramebufferSizeCallback(window, screenRatioCallback);
    glfwMakeContextCurrent(window);

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