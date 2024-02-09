#ifndef _SCENE_H
#define _SCENE_H

#include <iostream>
#include "entity.h"
#include <vector>
#include <algorithm>
#include <glad/glad.h>

class Scene
{
private:
    std::vector<Entity *> entities;
    Entity *skybox;

public:
    Scene();
    ~Scene();

    void Update(float deltaTime);
    void Render();

    void AddEntity(Entity *entity);
    void AddSkybox(Entity *skybox);
    void RemoveEntity(Entity *entity);
};

#endif