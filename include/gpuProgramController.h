#ifndef _GPUPROGRAMCONTROLLER_H
#define _GPUPROGRAMCONTROLLER_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "textureLoader.h"
#include "lightInterpolationType.h"

class GpuProgramController
{
public:
    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;
    GLint bbox_min_uniform;
    GLint bbox_max_uniform;
    GLint texture_projection_type;
    GLint interpolation_type;
    GLint is_texture_skybox;
    GLint texture_id;

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
};

#endif