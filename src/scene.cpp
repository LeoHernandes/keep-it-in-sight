#include "scene.h"

Scene::Scene()
{
    // não sei
}

Scene::~Scene()
{
    for (Entity *entity : this->entities)
    {
        delete entity;
    }
}

Scene *Scene::Update(float deltaTime)
{
    for (Entity *entity : this->entities)
    {
        entity->Update(deltaTime);
    }
    return this;
}

void Scene::Render()
{
    for (Entity *entity : this->entities)
    {
        entity->Render(this->camera);
    }
}

void Scene::RemoveEntity(Entity *entity)
{
    this->entities.remove(entity);
    delete entity;
}