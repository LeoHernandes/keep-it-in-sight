#ifndef SCENE_H
#define SCENE_H

#include <list>

// =========== TODO: IMPLEMENTAR ESSA CLASSE ===========
//#include "Entity.h" 

// =========== TODO: IMPLEMENTAR ESSA CLASSE ===========
//#include "Camera.h"

class Scene
{
    protected:
        std::list<Entity*> entities;

    public:
        Camera* camera;

        Scene();
        virtual ~Scene();

        virtual Scene* Update(float deltaTime);
        virtual void Render();

        template<typename T>
        T* AddEntity(T* entity)
        {
            this->entities.push_back(entity);
            return entity;
        }
        void RemoveEntity(Entity* entity);
};

#endif