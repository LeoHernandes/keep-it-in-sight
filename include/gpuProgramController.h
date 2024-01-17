#ifndef _GPUPROGRAMCONTROLLER_H
#define _GPUPROGRAMCONTROLLER_H

#include <glad/glad.h>

class GpuProgramController
{
public:
    GLint model_uniform;
    GLint view_uniform;
    GLint projection_uniform;

    GpuProgramController(GLuint gpu_program_id);
};

#endif