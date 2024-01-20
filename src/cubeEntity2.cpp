#include "cubeEntity2.h"

CubeEntity2::CubeEntity2(std::string name, GpuProgramController *gpu_controller, float totalDuration, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4) : Entity(name, gpu_controller)
{
    cubicBezier = new CubicBezier(totalDuration, p1, p2, p3, p4);

    GLfloat model_coefficients[] = {
        -0.5f, 0.0f, 0.5f, 1.0f,   // vertex 0
        -0.5f, -1.0f, 0.5f, 1.0f,  // vertex 1
        0.5f, -1.0f, 0.5f, 1.0f,   // vertex 2
        0.5f, 0.0f, 0.5f, 1.0f,    // vertex 3
        -0.5f, 0.0f, -0.5f, 1.0f,  // vertex 4
        -0.5f, -1.0f, -0.5f, 1.0f, // vertex 5
        0.5f, -1.0f, -0.5f, 1.0f,  // vertex 6
        0.5f, 0.0f, -0.5f, 1.0f,   // vertex 7
    };

    GLfloat color_coefficients[] = {
        1.0f, 0.5f, 0.0f, 1.0f, // vertex 0
        1.0f, 0.5f, 0.0f, 1.0f, // vertex 1
        0.0f, 0.5f, 1.0f, 1.0f, // vertex 2
        0.0f, 0.5f, 1.0f, 1.0f, // vertex 3
        1.0f, 0.5f, 0.0f, 1.0f, // vertex 4
        1.0f, 0.5f, 0.0f, 1.0f, // vertex 5
        0.0f, 0.5f, 1.0f, 1.0f, // vertex 6
        0.0f, 0.5f, 1.0f, 1.0f, // vertex 7
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Position
    GLuint VBO_model_coefficients_id;
    glGenBuffers(1, &VBO_model_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_model_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(model_coefficients), model_coefficients);
    GLuint location = 0;
    GLint number_of_dimensions = 4;
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Color
    GLuint VBO_color_coefficients_id;
    glGenBuffers(1, &VBO_color_coefficients_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_color_coefficients_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_coefficients), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color_coefficients), color_coefficients);
    location = 1;
    number_of_dimensions = 4;
    glVertexAttribPointer(location, number_of_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint indices[] = {
        0, 1, 2, // triangle 1
        7, 6, 5, // triangle 2
        3, 2, 6, // triangle 3
        4, 0, 3, // triangle 4
        4, 5, 1, // triangle 5
        1, 5, 6, // triangle 6
        0, 2, 3, // triangle 7
        7, 5, 4, // triangle 8
        3, 6, 7, // triangle 9
        4, 3, 7, // triangle 10
        4, 1, 0, // triangle 11
        1, 6, 2, // triangle 12
    };

    GLuint indices_id;
    glGenBuffers(1, &indices_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);
    glBindVertexArray(0);

    this->vertex_array_object_id = VAO;
}

void CubeEntity2::Update(float deltaTime)
{
    cubicBezier->Update(deltaTime);
    position = cubicBezier->GetPoint();
}

void CubeEntity2::Render()
{
    glBindVertexArray(vertex_array_object_id);
    glm::mat4 model = Matrices::Identity() * Matrices::Translate(position.x, position.y, position.z);
    glUniformMatrix4fv(gpu_controller->model_uniform, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void *)0);

    model = Matrices::Identity();
    glBindVertexArray(0);
}