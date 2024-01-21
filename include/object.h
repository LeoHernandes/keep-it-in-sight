#ifndef _OBJECT_H
#define _OBJECT_H

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tiny_obj_loader.h>

#include <cassert>
#include <string>
#include "entity.h"
#include "matrices.h"
#include "collisions.h"

class Object
{
private:
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    void ComputeNormals();
    void BuildVAO();

public:
    Object(const char *filename);
    GLuint vertex_array_object_id;
    size_t first_index;
    size_t num_indices;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
};

#endif //_OBJECT_H