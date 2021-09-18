#ifndef ENTITY_GUI
#define ENTITY_GUI

#include "entity.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

//generic gui it can have as much quads as we want
class EntityGui : public Entity {
public:
	//mesh, texture and shader for all quads
	std::vector<Mesh> quads;
	Shader* shader;
	Texture* texture;

	//if we want a backgruond image, it have to be the firts quad
	bool haveBack = false;
	Texture* textureBack;

	int numberOfquads = 0;//total number of quads
	float factorOfDistortion = 1; //diference of size between x and y

	std::vector <Vector4> icons; //the position of the icons in the texture
	std::vector < Vector2> screenPosition; //the position of the icons on the screen
	std::vector <int> partToPaint; //part of the icon to be painted
	std::vector <Vector2> sizes; //size of the icon to be painted
	std::vector <Vector4> color; //color of the quad


	EntityGui();
	//set the specific properties of the quads
	virtual void setVariables() {};
	//render all quads int the screen
	virtual void render();
	//update the quads with the current properties
	virtual void update(float elapsed_time);
};

#endif
