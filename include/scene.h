#ifndef _SCENE_H
#define _SCENE_H

#include <list>
#include "entity.h"
#include "camera.h"

class Scene
{
protected:
    std::list<Entity *> entities;

public:
    Camera *camera;

    Scene();
    virtual ~Scene();

    virtual Scene *Update(float deltaTime);
    virtual void Render();

    template <typename T>
    T *AddEntity(T *entity)
    {
        this->entities.push_back(entity);
        return entity;
    }
    void RemoveEntity(Entity *entity);
};

#endif