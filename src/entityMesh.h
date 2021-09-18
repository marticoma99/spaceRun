#ifndef ENTITY_MESH_H
#define ENTITY_MESH_H

#include "entity.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "light.h"

//Represent all the entities with a mesh representation
class EntityMesh : public Entity {
public:

    //variables that deffine the mesh
    Mesh* mesh;
    Texture* texture;
    Shader* shader;

    //position and rotation of the player
    Vector3 position = Vector3(0,0,0);
    float angle = 0;

    bool isAnimated = false; //if is a animated item
    bool isProjectile = false; //if is a projectile the update is diferent
    int shotType = 0; //type 0 shots by enemy and type 1 shots by player
    float totalTime = 0;

    EntityMesh(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent);
    EntityMesh();

    //render the actual mesh and the childreen
    virtual void render();
    //general function to render the mesh
    void renderMesh();
    //update the actual mesh and the childreen
    virtual void update(float elapsed_time);
    //true if the mesh is seen by the camara
    bool isMeshOnScene();
    //true if the mesh is coliding with some object of the class world
    bool isColidingWorld(Vector3 futurePos, Vector3* coll, float radi);
    //for animated objects (for the collectable meshes)
    void animationUpdate(float elapsed_time);
    //move the entity straight (used for projectiles)
    void moveLineal(float elapsed_time);
    //remove the projectile if it have a collision with the world
    bool removeProjectile(Vector3 futurePos);
    //test if the entity is colliding with an object (returns the index of the list if collision)
    int isOnObject(std::vector<EntityMesh*> objects);
    //opens the closed door of the level
    void openDoor();
    //check if is collding with the closed door
    bool isCollidingDoor(Vector3 futurePos, Vector3* coll, float radi);
    //shoot a projectile
    void shot(int type);
    //return true if is colliding with an enemy
    bool isCollidingEnemy(Vector3 position, Vector3* coll);
    //check collision with a projectile
    bool testProjectileCollision(std::vector<EntityMesh*>* aux);
    //return the type of the object that have around the position
    int isAroundObject(Vector3 position, int* pos);
};

#endif