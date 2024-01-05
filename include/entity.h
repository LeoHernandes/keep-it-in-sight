#ifndef _ENTITY_H
#define _ENTITY_H

#include "lookAtCamera.h"

class Entity
{
public:
    virtual ~Entity();
    virtual void Update(float deltaTime);
    virtual void Render(LookAtCamera *camera);
};

#endif