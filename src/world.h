#ifndef WORLD_H
#define WORLD_H

#define _CRT_SECURE_NO_WARNINGS

#include "entityMesh.h"
#include "entity.h"
#include "map.h"


//Reprsesnts the world map
class World : public Map{
public:
	
	int numMeshes = 16; //number ot mashes of the map
	int groundSize = 4; //number of tiles of the ground

	//rotations and translations for every mesh (only used by the walls)
	double orientations[20] = { 0, 180, -90, 90, -90, 0, 180, 90 , 90, -90, 0, 180 , 0, 180, -90, 90,  0, 180, -90, 90 };
	double translationX[20] = { 0.5, 0.5, 1.0, 0.0, 0.815, 0.315, 0.315, -0.185, -0.187, 0.813, 0.313, 0.313, 0.5, 0.5, 1.0, 0.0, 0.5, 0.5, 1.0, 0.0 };
	double translationY[20] = { 0.0, 1.0, 0.5, 0.5, 0.495, 0.0, 1.0, 0.495, 0.505, 0.505, 0, 1.0, 0.0, 1.0, 0.5, 0.5,  0.0, 1.0, 0.5, 0.5 };
	double signX [20] = { 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, 1, -1, -1, 1,  1, -1, -1, 1 };
	double signY[20] = { 1, -1, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1 ,  1, -1, 1, -1,  1, -1, 1, -1 };

	//the ground vector (contains all the pieces of the ground)
	std::vector<EntityMesh*> ground;
	Mesh* groundM = NULL;
	Texture* groundT = NULL;

	World(Entity* _root, int level);

	//load all meshes and textures
	void loadData();
	void setMesh(int type, int x, int y);
	void setGround();
};

#endif