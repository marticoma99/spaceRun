#include "entityPlayer.h"
#include "input.h"
#include "scene.h"
#include "game.h"
#include "stage.h"
#include "sound.h"

EntityPlayer::EntityPlayer(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent) :
	EntityMesh(mesh, texture, shader, model, parent) {
	//set the player position (y is always 0)
	position = Vector3(1.0f, 0.0f, 5.0f);
	this->model.setTranslation(position.x, 0, position.z);

	//initialize the movement properties
	playerParticle.vel = Vector3(0.0f, 0.0f, 0.0f);
	playerParticle.angle = 0.0f;
	playerParticle.velAngular = 0.0f;
	playerParticle.rotationSpeed = 90;
	playerParticle.friction = 0.04f;
}

//render the player and the childreen (wheels)
void EntityPlayer::render() {
	
	Camera* camera = Camera::current;
	Matrix44 model = this->getGlobalMatrix();

	//set the camera position according to the position of the player
	Vector3 eye = model * Vector3(0, camaraPos - 0.25 , camaraPos);
	Vector3 center = model * Vector3();
	Vector3 up = model.rotateVector(Vector3(0, 1, 0));
	camera->lookAt(eye, center, up);

	this->renderMesh();

	//call the render of the children
	for (int i = 0; i < children.size(); i++)
		children[i]->render();
}

//move the player with the cursors
void EntityPlayer::update(float elapsed_time) {

	checkMove(elapsed_time); //update the movement based in the input
	checkInput(); //make an action
	testObjects(elapsed_time); //test the collisions with the objects of the scene

	//update the player live and fuel
	Scene::instance->setLife(elapsed_time * Scene::instance->difficulty.LIFEGAIN);
	Scene::instance->setFuel(-elapsed_time * Scene::instance->difficulty.FUELLOSE);

	//chack if the player is in a win position
	checkWinState();

	//update the childreen
	for (int i = 0; i < children.size(); i++)
		children[i]->update(elapsed_time);
}

//check the input do an action if needed
void EntityPlayer::checkInput() {
	//shot a projectile
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) shot(1);
	//put a bridge in the radioactive river
	if (Input::wasKeyPressed(SDL_SCANCODE_Q)) {
		if (Scene::instance->numberOfBridge > 0) setBridge();
		else Sound::Play("data/sounds/wrong.wav", 0.1, 1);
	}
	//activate a lever
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) activateLever();
	//open the paper with lever order
	if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
		if (Scene::instance->haveMap) showLevers();
		else Sound::Play("data/sounds/wrong.wav", 0.1, 1);
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) Scene::instance->numberOfBridge ++;
	if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
		Scene::instance->vSequence = "321";
		//add the lever to the sequence and check if is valid
		if (Scene::instance->validSequence(7))
		{
			Sound::Play("data/sounds/win.wav", 0.5, 1);
			openDoor(); //open the door, so the player can go to the next level
		}
	}
}

//check and update the player movement
void EntityPlayer::checkMove(float elapsed_time) {
	//calculate the front vector
	Matrix44 r;
	r.setRotation(this->angle, Vector3(0, 1, 0));
	Vector3 front = r.rotateVector(Vector3(0, 0, -1));

	Vector3 velocity = playerParticle.vel; //actual player velocity
	int yaw = 0; //direction of rotation
	bool playerMove = false; //if the player have move

	//set the target position
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		playerMove = true;
		velocity = velocity + front * elapsed_time * Scene::instance->playerVelocity;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		playerMove = true;
		velocity = velocity - front * elapsed_time * Scene::instance->playerVelocity;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		playerMove = true;
		yaw = -1;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		playerMove = true;
		yaw = 1;
	}

	updateMove(elapsed_time, velocity, yaw, playerMove);
}

//update the player position and angle
void EntityPlayer::updateMove(float elapsed_time, Vector3 velocity, int yaw, bool playerMove) {

	float angularVelocity = playerParticle.velAngular;
	float futureAngle = angle;
	Vector3 futurePos = position;

	//if the shift key is pressed the player run
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) velocity = velocity * 1.02;
	if (playerMove) Sound::PlaySingle("data/sounds/move.wav", 0.75);
	else Sound::StopAudio("data/sounds/move.wav");

	//update the position with the new velocity
	velocity = velocity - velocity * playerParticle.friction;
	futurePos = position + velocity;

	//update the rotation with the new velocity
	angularVelocity += yaw * elapsed_time * playerParticle.rotationSpeed;
	angularVelocity -= angularVelocity * playerParticle.friction;
	futureAngle += DEG2RAD * angularVelocity * playerParticle.friction;

	//update the player with the new position if is valid
	updatePosition(futurePos, futureAngle, elapsed_time);

	//save the new velocities
	playerParticle.vel = velocity;
	playerParticle.velAngular = angularVelocity;
	Scene::instance->playerPos = position;
}

//set the player to the new position if the position is valid
void EntityPlayer::updatePosition(Vector3 futurePos,float futureAngle, float elapsed_time) {

	//direction of collision
	Vector3 coll;
	Vector3 collEnemy;

	//check if the player is colling with the scene (if not update the position)
	if (!isColidingWorld(futurePos, &coll, 0.25) && !isCollidingEnemy(futurePos, &coll) && !isCollidingDoor(futurePos, &coll, 0.25)) {
		Scene* scene = Scene::instance;
		position = futurePos;
		angle = futureAngle;
	}
	else {
		position = position - normalize(coll - futurePos) * elapsed_time;
		position.y = 0;
		playerParticle.vel = Vector3(0, 0, 0);
	}

	//set the new position
	model.setTranslation(position.x, 0, position.z);
	model.rotate(angle, Vector3(0, 1, 0));
}

//test collisions with all objects in the player position
void EntityPlayer::testObjects(float elapsed_time) {
	
	//check the collisions with all types of objects of the scene
	int i = checkCollectable(NULL);
	Scene* scene = Scene::instance;
	if (i == 9 || i == 13) scene->setLife(-scene->difficulty.RADIOACTIVEDAMAGE * elapsed_time);
	if (i == 2) scene->numberOfBridge++;
	if (i == 8) scene->haveMap = true;
	if( i == 3) Scene::instance->setFuel(Scene::instance->difficulty.FUELQUANTITY);
	
	//if the player recive a shot decrement it's life
	if (testProjectileCollision(&Scene::instance->shotsEnemy))
		scene->setLife(-scene->difficulty.ENEMYDAMAGE);
		
}

//check if the player is colliding with a collectable of a list
int EntityPlayer::checkCollectable(int* pos) {
	Objects* ObjectsAux = Scene::instance->objects;

	//only check the objects near the player position
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {

			//the position of the cell that we are cheking
			int posCell = floor(position.x) + i + (floor(position.z) + j) * ObjectsAux->width;

			//check if the position is out of the scene
			if (posCell < 0 || posCell >= ObjectsAux->width * ObjectsAux->height ||
				ObjectsAux->cells[posCell].type == 0) continue;

			EntityMesh* aux = ObjectsAux->cells[posCell].mesh;
			if (aux == NULL) continue;

			//test the collision
			int type = ObjectsAux->cells[posCell].type; //type of the mesh checking the collision
			if (aux->mesh->testSphereCollision(aux->model, position + Vector3(0,0.25,0), 0.25, Vector3(), Vector3())) {
				if (type == 2 || type == 3 || type == 8) {
					//the collectable objects are destroied
					Scene::instance->root->rmvChild(aux);
					ObjectsAux->cells[posCell].type = 0;
					Sound::Play("data/sounds/coin.wav", 0.25, 1);
				}
				if (pos != NULL) *pos = posCell;
				return type;
			}
		}
	}
	return -1;
}

//set a bridge in the radioactive river if its possible
void EntityPlayer::setBridge() {
	Objects* ObjectsAux = Scene::instance->objects;
	Scene* sceneAux = Scene::instance;
	int pos;
	//check if the player is near a possible bridge
	int type = isAroundObject(this->position, &pos);
	if (type == 13) {

		//remove the actual radioactive river
		sceneAux->root->rmvChild(ObjectsAux->cells[pos].mesh);
		Matrix44 model = ObjectsAux->cells[pos].mesh->model;
		delete ObjectsAux->cells[pos].mesh;

		//set the new bridge
		EntityMesh* newEntity = new EntityMesh(ObjectsAux->meshes[0], ObjectsAux->textures[0], shader, model, sceneAux->root);
		sceneAux->objects->cells[pos].type = 1;
		sceneAux->root->addChild(newEntity);
		sceneAux->numberOfBridge--;
		Sound::Play("data/sounds/setObject.wav", 0.25, 1);
	}
	else Sound::Play("data/sounds/wrong.wav", 0.1, 1);
}

//activate a lever if the player is near to it
void EntityPlayer::activateLever() {

	int pos;
	//check the objects around the player
	int type = isAroundObject(this->position, &pos);
	//if is the type of a lever
	if (type == 4 || type == 5 || type == 6 || type == 7) {

		EntityMesh* lever = Scene::instance->objects->cells[pos].mesh;
		//animation when activating the lever
		lever->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
		Sound::Play("data/sounds/setObject.wav", 0.25, 1);

		//add the lever to the sequence and check if is valid
		if (Scene::instance->validSequence(type))
		{
			Sound::Play("data/sounds/win.wav", 0.5, 1);
			openDoor(); //open the door, so the player can go to the next level
		}
	}
}

//show the paper with the lever order
void EntityPlayer::showLevers() {
	Sound::Play("data/sounds/click.wav", 1.5, 1);
	float height=Game::instance->window_height;
	float width = Game::instance->window_width;
	//if is showing the paper unshow it
	if (Scene::instance->isShowingImage)Scene::instance->unSowImage();
	//show the paper
	else Scene::instance->showImage("data/gui/levers.png", Vector2(width - 125, height - 125), Vector2(200, 200), 1000);
}

//check if the player is on a win position
void EntityPlayer::checkWinState() {
	if (position.x >= 27.0f && position.x <= 31.0f && position.z >= 27.0f && position.z <= 31.0f) {
		Scene::instance->win = true;
	}
}