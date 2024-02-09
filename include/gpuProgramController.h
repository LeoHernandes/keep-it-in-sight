#ifndef _GPUPROGRAMCONTROLLER_H
#define _GPUPROGRAMCONTROLLER_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "textureLoader.h"
#include "lightInterpolationType.h"

class GpuProgramController
{
private:
    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;
    GLint bbox_min_uniform;
    GLint bbox_max_uniform;
    GLint texture_coordinates_type;
    GLint interpolation_type;
    GLint is_texture_skybox;
    GLint texture_id;

public:
    GpuProgramController(GLuint gpu_program_id);

    void DrawElements(
        GLuint VAO_id,
        glm::mat4 model,
        TextureCoordinatesType text_coords_type,
        LightInterpolationType light_type,
        GLint texture_id, glm::vec3 bbox_min,
        glm::vec3 bbox_max,
        size_t num_indices,
        size_t first_index);

    void DrawSkybox(
        GLuint VAO_id,
        glm::mat4 model,
        GLint tex_id,
        glm::vec3 bbox_min,
        glm::vec3 bbox_max,
        size_t num_indices,
        size_t first_index);

    void SendPlayerCameraMatrices(glm::mat4 view, glm::mat4 projection);
};

#endif