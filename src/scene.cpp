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
        entity->Update(deltaTime);
    }
}

void Scene::AddSkybox(Entity *skybox)
{
    this->skybox = skybox;
}

void Scene::AddEntity(Entity *entity)
{
    entities.push_back(entity);
}

void Scene::Render()
{
    if (skybox != NULL)
    {
        skybox->Render();
    }

    for (const auto &entity : entities)
    {
        entity->Render();
    }
}

void Scene::RemoveEntity(Entity *entity)
{
    auto iter = std::find(entities.begin(), entities.end(), entity);

    if (iter != entities.end())
        entities.erase(iter);
}