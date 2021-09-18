#include "entityMesh.h"
#include "input.h"
#include "scene.h"
#include "sound.h"

EntityMesh::EntityMesh(Mesh* m, Texture* t, Shader* s, Matrix44 model, Entity* parent) : Entity(model, parent) {
	mesh = m;
	texture = t;
	shader = s;
}

EntityMesh::EntityMesh() : Entity() {
	mesh = NULL;
	texture = NULL;
	shader = NULL;
}

//render tha actual mesh and call the render of the child
void EntityMesh::render() {
	//render the mesh
	this->renderMesh();

	for (int i = 0; i < children.size(); i++)
		children[i]->render();
}

//function general for rendering a mesh
void EntityMesh::renderMesh() {

	//check if the mesh is on scene
	if (!isMeshOnScene()) return;

	Camera* camera = Camera::current;
	Matrix44 model = this->getGlobalMatrix();

	shader->enable();

	//upload uniforms to the shader
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setVector3("eye", camera->eye);
	shader->setTexture("u_texture", texture, 0);

	//use the light and material from the scene
	Scene::instance->material->uploadShader(shader);
	Scene::instance->light->uploadShader(shader);

	//render the mesh
	mesh->render(GL_TRIANGLES);
	mesh->disableBuffers(shader);
}

//test if a mesh is being seen by the player
bool EntityMesh::isMeshOnScene() {
	
	Camera* camera = Camera::current;
	Vector3 pos = Vector3();

	//if the mesh is not in camera frustrum range we ignore it
	pos = this->getGlobalMatrix() * mesh->box.center;
	if (camera->testSphereInFrustum(pos, mesh->radius)) return true;
	return false;
}

//update the actual mesh and call the update of the children
void EntityMesh::update(float elapsed_time) {
	//actions for specific meshes
	if (isAnimated) animationUpdate(elapsed_time);
	if (isProjectile)moveLineal(elapsed_time);

	//update the children
	for (int i = 0; i < children.size(); i++)
		children[i]->update(elapsed_time);
}

//check if a entity mesh is colliding with an object of the world
bool EntityMesh::isColidingWorld(Vector3 futurePos, Vector3* coll, float radi) {

	Vector3 collnorm;
	World* worldAux = Scene::instance->world;
	Vector3 posToMove = futurePos;
	
	//only check the objects between the player position
	for (int i = -3; i <= 3; i++) {
		for (int j = -3; j <= 3; j++) {
			//the position of the cell that we are cheking
			int posCell = floor(posToMove.x) + i + (floor(posToMove.z) + j) * worldAux->width;
			//check if the position is out of the scene
			if (posCell < 0 || posCell >= worldAux->width * worldAux->height || 
				worldAux->cells[posCell].type == 0) continue;

			//test the collision
			EntityMesh* aux = worldAux->cells[posCell].mesh;
			if (aux->mesh->testSphereCollision(aux->model, posToMove + Vector3(0,0.25,0), radi, *coll, collnorm)) return true;
		}
	}
	return false;
}

//update the elements with animation (for the collectable meshes)
void EntityMesh::animationUpdate(float elapsed_time) {
	//if the entity is not at scene we ignore it
	if (!isMeshOnScene()) return;

	//set the rotation and total time
	totalTime += elapsed_time;
	angle += 45 * elapsed_time * DEG2RAD;

	//set the movement up and down
	model.setTranslation(position.x, 0.05 * sin(90 * DEG2RAD * totalTime) + 0.5, position.z);
	//rotate the mesh
	model.rotate(angle, Vector3(0, 1, 0));

}

//move the projectile
void EntityMesh::moveLineal(float elapsed_time) {
	//calculate the front vector
	Matrix44 r;
	r.setRotation(this->angle, Vector3(0, 1, 0));
	Vector3 front = r.rotateVector(Vector3(0, 0, -1));

	//move the projectile (only in translation the rotate does not change)
	Vector3 futurePos = position + front * 10 * elapsed_time;

	if(!removeProjectile(futurePos)){
		//set the position and the model
		position = futurePos;
		model.setTranslation(position.x, 0, position.z);
		model.rotate(angle, Vector3(0, 1, 0));
	}
}

//remove the projectile if it collide with the world
bool EntityMesh::removeProjectile(Vector3 futurePos) {
	//if the object collide with the world the entity is destroied
	if (isColidingWorld(futurePos - Vector3(0, 0.75, 0), new Vector3(), 0.1)) {

		Scene* scene = Scene::instance;
		int pos = 0;
		//remove the entity from the root
		scene->root->rmvChild(this);
		if (this->shotType == 0) {
			//remove the entity from vector of shots
			for (int i = 0; i < scene->shotsEnemy.size(); i++)
				if (scene->shotsEnemy[i] == this) pos = i;
			scene->shotsEnemy.erase(scene->shotsEnemy.begin() + pos);
			scene->root->rmvChild(this);
			delete this;
		}
		if (this->shotType == 1) {
			//remove the entity from vector of shots
			for (int i = 0; i < scene->shotsPlayer.size(); i++)
				if (scene->shotsPlayer[i] == this) pos = i;
			scene->shotsPlayer.erase(scene->shotsPlayer.begin() + pos);
			scene->root->rmvChild(this);
			delete this;
		}
		return true;
	}
	return false;
}

//check collisions in list of entity mesh return the index of the entity with collision
int EntityMesh::isOnObject(std::vector<EntityMesh*> objects) {
	Vector3 collnorm;
	Vector3 coll;
	//for every element of the list
	for (int i = 0; i < objects.size(); i++) {
		//if the object is not near from the player the collision is not checked
		if (objects[i]->position.x > position.x + 1 && objects[i]->position.x < position.x - 1) continue;
		if (objects[i]->position.z > position.z + 1 && objects[i]->position.z < position.z - 1) continue;
		if (objects[i] == this) continue;
		//check the collision and return the index of the enity if collision is found
		if (objects[i]->mesh->testSphereCollision(objects[i]->model, position, 0.25, coll, collnorm)) return i;
	}
	//return -1 if no collision found
	return -1;
}

//open the level closed door
void EntityMesh::openDoor() {

	Scene* sceneAux = Scene::instance;
	
	int pos = sceneAux->doorPos.x + sceneAux->doorPos.z * sceneAux->objects->width; //position of the closed door
	if (sceneAux->objects->cells[pos].type == 0) return; //if the door is already open

	//remove the door
	EntityMesh* meshAux = sceneAux->objects->cells[pos].mesh;
	sceneAux->root->rmvChild(meshAux);
	sceneAux->objects->cells[pos].type = 0;
	delete meshAux;
}

//check if is colliding with a door
bool EntityMesh::isCollidingDoor(Vector3 futurePos, Vector3* coll, float radi) {
	
	Scene* sceneAux = Scene::instance;
	int pos = sceneAux->doorPos.x + sceneAux->doorPos.z * sceneAux->objects->width; //door position
	//if type is 0 it means that there is no object
	if (sceneAux->objects->cells[pos].type == 0) return false;

	//check the collision
	EntityMesh* meshAux = sceneAux->objects->cells[pos].mesh;
	if(meshAux->mesh->testSphereCollision(meshAux->model, futurePos, radi, *coll, Vector3())) return true;
	
	return false;
}

//create a new projectile
void EntityMesh::shot(int type) {

	Scene* scene = Scene::instance;

	//set the projectile to the position of the player
	Matrix44 newModel = model;
	newModel.translate(0, 0.75, 0);

	//create the projectile
	EntityMesh* aux = new EntityMesh(scene->projectileM, scene->projectileT, scene->shader, newModel, scene->root);
	aux->shotType = type;
	aux->isProjectile = true;

	//set the projectile to the position of the player
	aux->angle = this->angle;
	aux->position = this->position + Vector3(0, 0.75, 0);

	scene->root->addChild(aux);

	//set the projectile to the corresponding list
	if (type == 0)scene->shotsEnemy.push_back(aux);
	if (type == 1)scene->shotsPlayer.push_back(aux);

	Sound::Play("data/sounds/shot.wav", 0.5, 5);
}

//check if is colliding with an enemy
bool EntityMesh::isCollidingEnemy(Vector3 position, Vector3* coll) {
	
	//all possible enemies
	std::vector<EntityMesh*>  enemy1 = Scene::instance->sEnemy1;
	std::vector<EntityMesh*>  enemy2 = Scene::instance->sEnemy2;
	
	//test collisions with enemy 1
	for (int i = 0; i < enemy1.size(); i++) {
		if (enemy1[i] == this) continue;
		if (enemy1[i]->mesh->testSphereCollision(enemy1[i]->model, position + Vector3(0, 0.25, 0), 0.25, *coll, Vector3())) return true;
	}
	//test collisions with enemy 2
	for (int i = 0; i < enemy2.size(); i++) {
		if (enemy2[i] == this) continue;
		if (enemy2[i]->mesh->testSphereCollision(enemy2[i]->model, position + Vector3(0, 0.25, 0), 0.25, *coll, Vector3())) return true;
	}
	return false;
}

//test collisions with the meshes of the vector aux
bool EntityMesh::testProjectileCollision(std::vector<EntityMesh*>* aux) {
	
	int pos = -1;
	pos = isOnObject(*aux);
	if (pos != -1) {
		//remove the projectile
		delete *(aux->begin() + pos);
		Scene::instance->root->rmvChild(*(aux->begin() + pos));
		aux->erase(aux->begin() + pos);
		return true;
	}
	return false;
}

//check if the position is around an objects
int EntityMesh::isAroundObject(Vector3 position, int* pos) {
	Objects* ObjectsAux = Scene::instance->objects;
	Scene* sceneAux = Scene::instance;
	int type = -1; // if no bject found return -1
	//check around the position
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			//the position of the cell that we are cheking
			int posCell = floor(position.x) + i + (floor(position.z) + j) * ObjectsAux->width;
			int auxType = ObjectsAux->cells[posCell].type;
			//only check the types we need
			if (auxType <= 0 || auxType == 9 || auxType > 13 || (auxType <= 11 && auxType >= 8) || auxType == 3 || auxType == 2) continue;
			*pos = posCell;
			type = ObjectsAux->cells[posCell].type;
		}
	}
	//return the type that the player have around
	return type;
}