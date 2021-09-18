#ifndef Objects_H
#define Objects_H

#define _CRT_SECURE_NO_WARNINGS

#include "entityMesh.h"
#include "entity.h"
#include "map.h"

//Reprsesnts the objecs of a level
class Objects : public Map {
public:
	//possible entities types
	enum { bridge, collectableBridge, fuel, 
		lever1, lever2, lever3, lever4, paper, radioactive, 
		enemy1, enemy2, closedDoor, possibleBridge };

	int numMeshes = 12; //number ot mashes of the map


	Objects(Entity* _root, int level);

	//load all meshes and textures
	void loadData();
	//set a mesh to a given type
	void setMesh(int type, int x, int y);
	//load all needed meshes
	void loadMeshes();
	//load all needed textures
	void loadTextures();
};

#endif