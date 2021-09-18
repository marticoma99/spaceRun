#include "entityGuiMenu.h"
#include "scene.h"
#include "game.h"
#include "input.h"

//set all the information of the menu GUI
EntityGuiMenu::EntityGuiMenu(const char* name) {

	numberOfquads = 5;
	haveBack = true;

	//general shader and textures for the menu gui
	shader = Shader::Get("data/shaders/gui.vs", "data/shaders/gui.fs");
	texture = Texture::Get(name);
	textureBack = Texture::Get("data/gui/background.png");

	setVariables(); //upload the variables to the quads

	//create the quads
	for (int i = 0; i < numberOfquads; i++) {

		color.push_back(Vector4(1, 1, 1, 1)); //color of the quad
		partToPaint.push_back(2048); //part of the icon to be painted

		//create the quad
		Mesh aux;
		aux.createQuad(screenPosition[i].x, screenPosition[i].y, sizes[i].x * factorOfDistortion, sizes[i].y, false);
		quads.push_back(aux);
	}
}

//upload the variables of the quads
void EntityGuiMenu::setVariables() {

	float width = Game::instance->window_width;
	float height = Game::instance->window_height;

	//the position of the icons in the texture
	icons.push_back(Vector4(0, 0, 1, 1));
	icons.push_back(Vector4(0, 0.5, 1, 0.5));
	icons.push_back(Vector4(0, 0, 0.5, 0.25));
	icons.push_back(Vector4(0.5, 0, 0.5, 0.25));
	icons.push_back(Vector4(0, 0.25, 0.5, 0.25));

	//the position of the icons on the screen
	screenPosition.push_back(Vector2(width / 2, height / 2));
	screenPosition.push_back(Vector2(width / 2, 150));
	screenPosition.push_back(Vector2(width / 2, 300));
	screenPosition.push_back(Vector2(width / 2, 400));
	screenPosition.push_back(Vector2(width / 2, 500));

	//size of the icon to be painted
	sizes.push_back(Vector2(width, height));
	sizes.push_back(Vector2(500, 300));
	sizes.push_back(Vector2(200, 100));
	sizes.push_back(Vector2(200, 100));
	sizes.push_back(Vector2(200, 100));
}

//update the quads information
void EntityGuiMenu::update(float elapsed_time) {
	float width = Game::instance->window_width;
	float height = Game::instance->window_height;

	sizes[0] = Vector2(width, height);
	screenPosition[0] = Vector2(width / 2, height / 2);
	screenPosition[1] = Vector2(width / 2, 150);
	screenPosition[2] = Vector2(width / 2, 300);
	screenPosition[3] = Vector2(width / 2, 400);
	screenPosition[4] = Vector2(width / 2, 500);
	
	isOnMouse();

	EntityGui::update(elapsed_time);
}

//check in which button is the mouse
int EntityGuiMenu::isOnMouse(){
	int pos = -1;
	Vector2 mouse = Input::mouse_position;
	//the three buttons update the color if the mouse is on
	for (int i = 2; i <= 5; i++) {
		if (mouse.x > screenPosition[i].x - sizes[i].x / 2 && mouse.x < screenPosition[i].x + sizes[i].x / 2 &&
			mouse.y > screenPosition[i].y - sizes[i].y / 2 && mouse.y < screenPosition[i].y + sizes[i].y / 2) {
			color[i] = Vector4(0.7, 0.7, 0.7, 1);
			//retrun which button have been pressed (possible returns are 1,2,3)
			pos = i - 1;
		}
		else color[i] = Vector4(1, 1, 1, 0.9);
	}
	return pos;
}