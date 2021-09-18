#include "map.h"
#include <stdio.h>
#include <cassert>
#include "scene.h"

//load the map from a file
bool Map::loadWorld(const char* fileName) {

	FILE* file = fopen(fileName, "rb");
	if (file == NULL) //not found
	{
		return false;
	}
	mapHeader header;
	fread(&header, sizeof(mapHeader), 1, file);

	assert(header.bytes == 1);

	unsigned char* auxCells = new unsigned char[header.w * header.h];
	fread(auxCells, header.bytes, header.w * header.h, file);

	width = header.w;
	height = header.h;

	Scene* scene = Scene::instance;

	if (cells != NULL) delete[] cells;
	cells = new cellsType[width * height];


	//put the information of the cells into the array of cells
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

			//set the type of the mesh
			int type = auxCells[j * width + i];
			cells[j * width + i].type = type;

			//set the cell x and y position
			cells[j * width + i].x = i;
			cells[j * width + i].y = j;

			if (type == 0) continue;

			setMesh(type, i, j);
		}
	}
	delete[] auxCells;
	return true;
}