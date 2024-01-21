#include "scene.h"

Scene::~Scene()
{
    entities.clear();
}

void Scene::Update(float deltaTime)
{
    for (const auto &entity : entities)
    {
        entity.second->Update(deltaTime);
    }
}

void Scene::AddEntity(Entity *entity)
{
    if (entities.find(entity->name) == entities.end())
    {
        entities[entity->name] = entity;
    }
    else
    {
        std::cerr << "[ERROR] AddEntity: an entity of name " << entity->name << " already exists in the scene" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Scene::Render()
{
    for (const auto &entity : entities)
    {
        entity.second->Render();
    }
}

void Scene::RemoveEntity(Entity *entity)
{
    entities.erase(entity->name);
}