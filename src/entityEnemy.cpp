#include "entityEnemy.h"
#include "scene.h"
#include "sound.h"

EntityEnemy::EntityEnemy(Mesh* m, Texture* t, Shader* s, Matrix44 model, Entity* parent, int type)
	:EntityMesh(m, t, s, model, parent) {
	this->type = type;
}

void EntityEnemy::update(float elapsed_time){
	
	//call the apropiate state
	if (state == IDLE) idleState(elapsed_time);
	if (state == ALERT) alertState(elapsed_time);
	if (state == COMBAT) combatState(elapsed_time);

	//check if have collision with a projectile
	checkProjectileCollision();
}

//check if the enemy have been collide by a projectile
void EntityEnemy::checkProjectileCollision() {
	
	if (testProjectileCollision(&Scene::instance->shotsPlayer)) {

		Sound::StopAudio("data/sounds/forceField.wav");
		Scene::instance->playerVelocity = Scene::instance->playerVelocityLevel;

		live -= Scene::instance->difficulty.PLAYERDAMAGE; // update the enemy live

		if (live < 0) {

			Sound::Play("data/sounds/die.wav", 0.35, 1);

			//remove the enemy
			std::vector<EntityMesh*>* aux;
			if (type == 1) aux = &Scene::instance->sEnemy1;
			if (type == 2) aux = &Scene::instance->sEnemy2;
			for (int i = 0; i < aux->size(); i++)
				if (*(aux->begin() + i) == this) aux->erase(aux->begin() + i);
			Scene::instance->root->rmvChild(this);
			delete this;
		}
	}
}

//rotate the enemy to the player
void EntityEnemy::rotateToPlayer(float elapsed_time) {
	
	Scene* scene = Scene::instance;

	//the vector to the player pos
	Vector2 toTarget = Vector2(scene->playerPos.x-position.x, scene->playerPos.z - position.z).normalize();
	float angle_in_rad  = atan2(toTarget.y, toTarget.x);//angle between the x axis
	if (angle_in_rad < 0) { angle_in_rad += 2 * M_PI; } //set the angle between pi and -pi

	//front vector of the enemy
	Matrix44 r;
	r.setRotation(this->angle, Vector3(0, 1, 0));
	Vector3 front = r.rotateVector(Vector3(0, 0, -1));
	Vector2 front2D = Vector2(front.x,front.z).normalize();

	float angle_in_rad_front = atan2(front2D.y, front2D.x); //angle between the x axis
	if (angle_in_rad_front < 0) { angle_in_rad_front += 2 * M_PI; } //normalize the angle
	float rotationAngle = angle_in_rad - angle_in_rad_front; //calculate the angle to rotate

	//if the angle to the player is between 0.2 and -0.2 the enemy can see the player
	if (rotationAngle < 0.2 && rotationAngle > -0.2) moveToPlayer(elapsed_time, rotationAngle);
	//if the angle between the player and the enemy is not between 0.15 and -0.15 change the state to combat
	if ((rotationAngle < 0.15 && rotationAngle > -0.15)) state = COMBAT;
	
	//apply the rotation
	model.rotate(rotationAngle * elapsed_time * rotateVelocity, Vector3(0, 1, 0));
	this->angle += rotationAngle * elapsed_time * rotateVelocity;
}

//move the enemy to the direction front
void EntityEnemy::moveToPlayer(float elapsed_time, float rotateAngle) {
	//calculate the front vector
	Matrix44 r;
	r.setRotation(this->angle, Vector3(0, 1, 0));
	Vector3 front = r.rotateVector(Vector3(0, 0, -1));

	//the enemy move in the direction of front vector
	Vector3 distance2Player = position - Scene::instance->playerPos;
	Vector3 futurePos = position + front * elapsed_time * abs(distance2Player.length()) * velocity;
	
	//if collision move the enemy away from the collision
	Vector3 aux;
	if (isColidingWorld(futurePos, &aux, 0.25)) futurePos = position - normalize(aux - position)* elapsed_time;
	if (isCollidingEnemy(futurePos, &aux))	futurePos = position - normalize(aux - position) * elapsed_time;

	//if the enemy is too near to the player the enemy stops moving
	if (abs(distance2Player.x) < 1 && abs(distance2Player.z) < 1) return;

	//update the translation and set the rotation
	position = futurePos;
	position.y = 0;
	model.setTranslation(position.x, 0, position.z);
	model.rotate(angle, Vector3(0, 1, 0));
}

//shot a projectile to the player direction
void EntityEnemy::enemyShot(float elapsed_time) {

	totalTime += elapsed_time;
	float time = totalTime - timeShots;
	//check if time to the previous shot is valid
	if ((totalTime - timeShots) < Scene::instance->difficulty.TIMEBETWEENSHOTS)	return;

	//set the initial time and shot
	timeShots = totalTime;
	shot(0);
}

//return true if the enemy can see the player
bool EntityEnemy::canSeePlayer() {

	Scene* scene = Scene::instance;
	//distance to the player position
	float distance2Player = Vector2(scene->playerPos.x - position.x, scene->playerPos.z - position.z).length();
	//if the enemy is away from the player it can see it
	if (distance2Player > visionRange) return false;

	Vector3 vector2player = (scene->playerPos - position).normalize();
	int halfVisionRange = visionRange / 2;
	World* worldAux = Scene::instance->world;

	//check if the enemy can see the player at halfVisionRange distance
	for (int i = -halfVisionRange; i <= halfVisionRange; i++) {
		for (int j = -halfVisionRange; j <= halfVisionRange; j++) {
			int posCell = floor(position.x) + i + (floor(position.z) + j) * worldAux->width; //get the cell
			//check if the position is out of the scene
			if (posCell < 0 || posCell >= worldAux->width * worldAux->height || worldAux->cells[posCell].type == 0) continue;

			//test if there are obstacles between the player end the enemy (check each cell)
			EntityMesh* aux = worldAux->cells[posCell].mesh;
			if(aux->mesh->testRayCollision(aux->model, position, vector2player, Vector3(), Vector3(), distance2Player)) return false;
		}
	}
	return true;
}

//the enemy go straight and if it founds a wall it rotate
void EntityEnemy::rotateRandom(float elapsed_time) {
	//calculate the front vector
	Matrix44 r;
	r.setRotation(this->angle, Vector3(0, 1, 0));
	Vector3 front = r.rotateVector(Vector3(0, 0, -1));

	//go straight
	Vector3 futurePos = position + front * elapsed_time  * velocity * 2;

	//if collision rotate
	Vector3 aux;
	if (isColidingWorld(futurePos, &aux, 0.5)) {
		futurePos = position - normalize(aux - position) * elapsed_time;
		angle += elapsed_time * 2;
	}
	if (isCollidingEnemy(futurePos, &aux)) {
		futurePos = position - normalize(aux - position) * elapsed_time;
		angle += elapsed_time;
	}

	//update the translation and set the rotation
	position = futurePos;
	position.y = 0;
	model.setTranslation(position.x, 0, position.z);
	model.rotate(angle, Vector3(0, 1, 0));
}

//attack for enemy 2 (force field atack)
void EntityEnemy::forceAtack(float elapsed_time) {
	Sound::PlaySingle("data/sounds/forceField.wav", 2);
	Scene* scene = Scene::instance;
	//decrement player velocity and life
	scene->playerVelocity = scene->playerVelocityLevel * 0.4;
	scene->setLife(-elapsed_time * scene->difficulty.ENEMYDAMAGE);
}

//state where the enemy go random
//if it see the enemy it change it state to alert
void EntityEnemy::idleState(float elapsed_time) {
	if(canSeePlayer()) state = ALERT;
	//set he player velocity to the initial value
	Scene* scene = Scene::instance;
	rotateRandom(elapsed_time);
}

//state that the enemy follow the player
//if it have good angle for soting its change the state to combat
//if the enemy can not see the player change its state to idle
void EntityEnemy::alertState(float elapsed_time) {
	if (!canSeePlayer()) {
		state = IDLE;
		Sound::StopAudio("data/sounds/forceField.wav");
		Scene::instance->playerVelocity = Scene::instance->playerVelocityLevel;
		return;
	}
	rotateToPlayer(elapsed_time);
}

//the enemy shoot a projectil in the direction it is looking
//if the enemy can not see the player change its state to idle
void EntityEnemy::combatState(float elapsed_time) {
	if (!canSeePlayer()) {
		state = IDLE;
		Sound::StopAudio("data/sounds/forceField.wav");
		Scene::instance->playerVelocity = Scene::instance->playerVelocityLevel;
		return;
	} 
	rotateToPlayer(elapsed_time);
	//do the action for corresponging to the type of enemy
	if (type == 1) enemyShot(elapsed_time);
	if (type == 2) forceAtack(elapsed_time);
}