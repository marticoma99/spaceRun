#include "objects.h"
#include "scene.h"
#include "entityEnemy.h"

Objects::Objects(Entity* _root, int level) {

	root = _root;

	//default shader
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//load all the meshes and textures required for the entities
	loadData();

	//true if the map is loaded correctly
	bool aux = false;
	//load the objects from a file each level have different map
	if (level == 0) aux = loadWorld("data/objects0.map");
	if (level == 1) aux = loadWorld("data/objects1.map");

	//if the objects have been loaded correctly
	if (aux == true) std::cout << " + Objects loaded correct\n";
	else std::cout << " + Could not load the bjects\n";
}


//load all the meshes and textures needed
void Objects::loadData() {
	loadMeshes();
	loadTextures();
}

//set an entity mesh for the type
void Objects::setMesh(int type, int x, int y) {
	//normalize the type (the real type 0 is for empty mesh)
	type = type - 1;
	//set the entity properties
	Matrix44 model;
	model.translate(x, 0, y);

	//in this positionthe player can put the bridge but initialy it is of type radioactive
	if (type == possibleBridge) type = radioactive;

	//the enemies have different type and are stored in a vector
	if (type == enemy1 || type == enemy2) {
		EntityEnemy* aux = new EntityEnemy(meshes[type], textures[type], shader, model, root, type - 8);
		aux->position = Vector3(x, 0, y);
		if (type == enemy1) Scene::instance->sEnemy1.push_back(aux);
		if (type == enemy2) Scene::instance->sEnemy2.push_back(aux);
		root->addChild(aux);
	}
	//all the other entities are stored in a array that each position represents a cell
	else {
		EntityMesh* aux = new EntityMesh(meshes[type], textures[type], shader, model, root);
		aux->position = Vector3(x, 0, y);
		//collectable objects are animated
		if (type == collectableBridge || type == fuel || type == paper) aux->isAnimated = true;
		//put the new entity mesh on it's cell
		cells[y * width + x].mesh = aux;
		root->addChild(aux);
	}
	if (type == closedDoor) Scene::instance->doorPos = Vector3(x, 0, y);
}

//upload the meshes for every type
void Objects::loadMeshes() {
	meshes[bridge] = Mesh::Get("data/objects/bridge.obj");
	meshes[collectableBridge] = Mesh::Get("data/objects/collectableBridge.obj");
	meshes[fuel] = Mesh::Get("data/objects/fuel.obj");
	meshes[lever1] = Mesh::Get("data/objects/lever.obj");
	meshes[lever2] = Mesh::Get("data/objects/lever.obj");
	meshes[lever3] = Mesh::Get("data/objects/lever.obj");
	meshes[lever4] = Mesh::Get("data/objects/lever.obj");
	meshes[paper] = Mesh::Get("data/objects/paper.obj");
	meshes[radioactive] = Mesh::Get("data/objects/radioactive.obj");
	meshes[enemy1] = Mesh::Get("data/objects/robot.obj");
	meshes[enemy2] = Mesh::Get("data/objects/robot.obj");
	meshes[closedDoor] = Mesh::Get("data/objects/closedDoor.obj");
}

//upload the textures for every type
void Objects::loadTextures() {
	textures[bridge] = Texture::Get("data/objects/bridge.tga");
	textures[collectableBridge] = Texture::Get("data/objects/collectableBridge.tga");
	textures[fuel] = Texture::Get("data/objects/fuel.tga");
	textures[lever1] = Texture::Get("data/objects/leverBlue.tga");
	textures[lever2] = Texture::Get("data/objects/leverGreen.tga");
	textures[lever3] = Texture::Get("data/objects/leverRed.tga");
	textures[lever4] = Texture::Get("data/objects/leverYellow.tga");
	textures[paper] = Texture::Get("data/objects/paper.tga");
	textures[radioactive] = Texture::Get("data/objects/radioactive.tga");
	textures[enemy1] = Texture::Get("data/objects/enemy1.tga");
	textures[enemy2] = Texture::Get("data/objects/enemy2.tga");
	textures[closedDoor] = Texture::Get("data/objects/closedDoor.tga");
}
