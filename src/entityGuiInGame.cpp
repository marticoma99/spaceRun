#include "entityGuiInGame.h"
#include "scene.h"
#include "game.h"

//set all the information of the game GUI
EntityGuiInGame::EntityGuiInGame() {

	Scene::instance->root->addChild(this);
	parent = Scene::instance->root;

	numberOfquads = 4;
	factorOfDistortion = 2;

	//general shader and texture
	shader = Shader::Get("data/shaders/gui.vs", "data/shaders/gui.fs");
	texture = Texture::Get("data/gui/icons.png");

	setVariables(); //upload the variables to the quad

	//create the quads
	for (int i = 0; i < numberOfquads; i++) {
		color.push_back(Vector4(1, 1, 1, 1)); // color of the quad
		sizes.push_back(Vector2(100, 100)); //size of the quad

		//create the quad
		Mesh aux;
		aux.createQuad(screenPosition[i].x, screenPosition[i].y, sizes[i].x * factorOfDistortion, sizes[i].y, false);
		quads.push_back(aux);
	}
}

//set the variables of each quad
void EntityGuiInGame::setVariables() {
	//the position of the icons in the texture
	icons.push_back(Vector4(0, 0.5, 0.5, 0.5));
	icons.push_back(Vector4(0.5, 0.5, 0.5, 0.5));
	icons.push_back(Vector4(0, 0, 0.5, 0.5));
	icons.push_back(Vector4(0.5, 0, 0.5, 0.5));

	//the position of the icons on the screen
	screenPosition.push_back(Vector2(125, 75));
	screenPosition.push_back(Vector2(350, 75));
	screenPosition.push_back(Vector2(125, 75));
	screenPosition.push_back(Vector2(350, 75));

	//part of the icon to be painted
	partToPaint.push_back(256);
	partToPaint.push_back(482);
	partToPaint.push_back(256);
	partToPaint.push_back(482);
}

//update the health and fuel part to paint
void EntityGuiInGame::update(float elapsed_time) {
	EntityGui::update(elapsed_time);

	//update the parameters
	partToPaint[0] = 256 - (128 - (int)Scene::instance->playerHealth) * 1.2;
	partToPaint[1] = 482 - (128 - (int)Scene::instance->playerFuel) * 1.2;
}