#ifndef _SCENE_H
#define _SCENE_H

#include <unordered_map>
#include <iostream>
#include "entity.h"

#include <glad/glad.h>

class Scene
{
private:
    std::unordered_map<std::string, Entity *> entities;
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