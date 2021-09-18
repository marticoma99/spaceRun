#include "world.h"

World::World(Entity* _root, int level){

	root = _root;

	//default shader
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//load all the meshes and textures required
	loadData();

	bool aux = false;
	//load the world from a file each level have different map
	if (level == 0) aux = loadWorld("data/world0.map");
	if (level == 1) aux = loadWorld("data/world1.map");

	//if the map have been loaded correctly
	if (aux == true) std::cout << " + Map world loaded correctly\n";
	else std::cout << " + Could not load map world\n";

	//set the ground
	setGround();
}


//load all the meshes and textures needed
void World::loadData() {

	std::string name;

	for (int i = 1; i <= numMeshes; i++) {

		//get the mesh of type i
		name = "data/world/" + std::to_string(i) + ".obj";
		Mesh* meshAux = Mesh::Get(name.c_str());
		meshes[i] = meshAux;

		//get the texture of type i
		name = "data/world/" + std::to_string(i) + ".tga";
		Texture* textureAux = Texture::Get(name.c_str());
		textures[i] = textureAux;
	}
}

//set a entity mesh to a type
void World::setMesh(int type, int x, int y) {
	//create the new entity mesh with the corresponding mesh type
	Matrix44 model;

	//normalize the type (the real type 0 is for empty mesh)
	type = type - 1;

	//applay the translation to the momdel
	if (type >= 0 && type < 20) {
		//set the rotation and translation based on stores data
		model.rotate(orientations[type] * DEG2RAD, Vector3(0, 1, 0));
		if (orientations[type] == 90 || orientations[type] == -90)
			model.translate(signX[type] * y + translationX[type], 0, signY[type] * x + translationY[type]);
		else model.translate(signX[type] * x + translationX[type], 0, signY[type] * y + translationY[type]);
	}
	else model.translate(x, 0, y);

	//set the mesh type (the first 20 meshes have 4 rotations)
	if (type >= 0 && type < 20) type = floor(type / 4) + 1;
	else {
		//normalize the type
		type = type - 14;
	}

	//put the new entity mesh on it's cell
	cells[y * width + x].mesh = new EntityMesh(meshes[type], textures[type], shader, model, root);
	root->addChild(cells[y * width + x].mesh);
}

//set the ground texture
void World::setGround() {

	//number of meshes needed
	int xSize = width / (groundSize * 2);
	int ySize = height / (groundSize * 2);

	groundT = Texture::Get("data/ground.tga");
	groundM = new Mesh;
	groundM->createPlane(groundSize);

	//set all the entity meshes
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			Matrix44 model = Matrix44();
			model.translate((groundSize - 0.5) + i * groundSize * 2, 0, (groundSize - 1) + j * groundSize * 2);
			EntityMesh* aux = new EntityMesh(groundM, groundT, shader, model, root);
			ground.push_back(aux);
			root->addChild(aux);
		}
	}
}
