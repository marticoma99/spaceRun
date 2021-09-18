#ifndef MAP_H
#define MAP_H

#include "entityMesh.h"

//type of a cell of the map, each position represents a cell
struct cellsType {
	EntityMesh* mesh = NULL;
	int type;
	int x;
	int y;
};

struct mapHeader {
	int w;
	int h;
	unsigned char bytes;
	unsigned char extra[7];
};

//generic .map load info
class Map {
public:

	//widht and height of the world number of cells
	int width;
	int height;

	//each position represents a cell
	cellsType* cells = NULL;

	//map the meshes and textures to a type
	std::map<int, Mesh*> meshes;
	std::map<int, Texture*> textures;

	// general shader
	Shader * shader;
	Entity* root;

	Map() {};
	//load a map
	bool loadWorld(const char* fileName);
	//virtual metods the be implemented by ojects and world
	virtual void setMesh(int type, int x, int y) {};
	virtual void loadData() {};
};

#endif
