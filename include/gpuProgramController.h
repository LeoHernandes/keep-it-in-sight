#ifndef _GPUPROGRAMCONTROLLER_H
#define _GPUPROGRAMCONTROLLER_H

#include <glad/glad.h>

class GpuProgramController
{
public:
    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;
    GLint bbox_min_uniform;
    GLint bbox_max_uniform;
    GLint texture_projection_type;
    GLint texture_id;

    GpuProgramController(GLuint gpu_program_id);
};

#endif