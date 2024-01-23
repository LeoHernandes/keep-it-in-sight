#include "scene.h"

Scene::Scene()
{
    this->skybox = NULL;
}

Scene::~Scene()
{
    entities.clear();
}

void Scene::Update(float deltaTime)
{
    if (skybox != NULL)
    {
        skybox->Update(deltaTime);
    }

    for (const auto &entity : entities)
    {
        entity.second->Update(deltaTime);
    }
}

void Scene::AddSkybox(Entity *skybox)
{
    this->skybox = skybox;
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
    if (skybox != NULL)
    {
        skybox->Render();
    }

    for (const auto &entity : entities)
    {
        entity.second->Render();
    }
}

void Scene::RemoveEntity(Entity *entity)
{
    entities.erase(entity->name);
}