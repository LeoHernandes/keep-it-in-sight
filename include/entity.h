#ifndef _ENTITY_H
#define _ENTITY_H

#include "camera.h"

class Entity
{
public:
    virtual ~Entity();
    virtual void Update(float deltaTime);
    virtual void Render(Camera *camera);
};

#endif