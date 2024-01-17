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
        std::cerr << "[ERROR] AddEntity: an entity of name " << entity->name << "already exists in the scene" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// TODO: get the model_uniform address from an adapter class for GPU methods
void Scene::Render(GLint model_uniform)
{
    for (const auto &entity : entities)
    {
        entity.second->Render(model_uniform);
    }
}

void Scene::RemoveEntity(Entity *entity)
{
    entities.erase(entity->name);
}