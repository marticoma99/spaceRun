
#ifndef ENTITY_ENEMY_H
#define ENTITY_ENEMY_H
#include "entity.h"
#include "mesh.h"
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include "material.h"
#include "light.h"
#include "entityMesh.h"

//Class that represents the enemy
class EntityEnemy : public EntityMesh{
public:
	
	int type = 0; //type of enemy (it can be 0 or 1)
	float velocity = 0.75; //velocity when going to the player
	float rotateVelocity = 3; //velocity of rotation
	float timeShots = 0; //time of the lest shot
	int visionRange = 6; //distance that an enemy can see the player

	enum {IDLE, ALERT, COMBAT};//posible enemy states
	int state = IDLE; //actual state
	int live = 100;

	EntityEnemy(Mesh* m, Texture* t, Shader* s, Matrix44 model, Entity* parent, int type);
	void update(float elapsed_time);
	//check if the enemy is colliding with a projectile if so destroy the enemy
	void checkProjectileCollision();
	//rotate the enemy to the player
	void rotateToPlayer(float elapsed_time);
	//move the anemy to the player direction
	void moveToPlayer(float elapsed_time, float rotateAngle);
	//shot the player
	void enemyShot(float elapsed_time);
	//return true if the enemy can see the player
	bool canSeePlayer();
	//if found a wall rotate
	void rotateRandom(float elapsed_time);
	//atack of the second enemy, makes the player go slower and decrement it's live
	void forceAtack(float elapsed_time);

	//the possibles states
	void idleState(float elapsed_time);
	void alertState(float elapsed_time);
	void combatState(float elapsed_time);
	
};

#endif
