#ifndef ENTITY_H
#define ENTITY_H

#include "framework.h"

//Base class that represents all the Entities
class Entity {
public:
    
    Matrix44 model; //reprsesnts the transformation of the entity
    Entity* parent; //the entity parent
    std::vector<Entity*> children; //the entities children

    Entity(Matrix44 _model, Entity* _parent);
    Entity();

    //function for render and update to be implemented by the children
    virtual void render();
    virtual void update(float elapsed_time);

    //add and remove childs
    void addChild(Entity* ent);
    void rmvChild(Entity* ent);

    //return the model matrix based on tha father model
    Matrix44 getGlobalMatrix();
};

#endif