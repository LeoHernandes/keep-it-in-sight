#ifndef ENTITY_H
#define ENTITY_H

// =========== TODO: IMPLEMENTAR ESSA CLASSE ===========
#include "Camera.h"

class Entity
{
    public:
        virtual ~Entity();
        virtual void Update(float deltaTime);
        virtual void Render(Camera* camera);
};

#endif