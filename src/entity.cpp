#include "entity.h"
#include <iostream>


Entity::Entity(Matrix44 _model, Entity* _parent) {
    model = _model;
    parent = _parent;
}

Entity::Entity() {
    //identity matrix
    model = Matrix44();
    parent = NULL;
};

//add a children to the current entity
void Entity::addChild(Entity* ent) {
    children.push_back(ent);
}

//remove a children from the current entity and put to it father
void Entity::rmvChild(Entity* ent) {
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == ent)
            children.erase(children.begin() + i);
    }
}

//return the model matrix based on the parent model
Matrix44 Entity::getGlobalMatrix() {
    if (parent)
        return model * parent->getGlobalMatrix();
    return model;
}

//call the render of the children
void Entity::render() {
    for (int i = 0; i < children.size(); i++)
        children[i]->render();
}

//call the update of the children
void Entity::update(float elapsed_time) {
       for (int i = 0; i < children.size(); i++)
        children[i]->update(elapsed_time);
}