#include "entityGuiText.h"
#include "scene.h"
#include "game.h"
#include "input.h"

//set all the information of the text GUI
EntityGuiText::EntityGuiText(const char* name) {

	numberOfquads = 4;
	haveBack = true;

	shader = Shader::Get("data/shaders/gui.vs", "data/shaders/gui.fs");
	texture = Texture::Get(name);
	textureBack = Texture::Get("data/gui/background.png");

	//upload the variables to the gui
	setVariables();

	//create the quads
	for (int i = 0; i < numberOfquads; i++) {

		color.push_back(Vector4(1, 1, 1, 1));//color of the quad
		partToPaint.push_back(2048); //part of the icon to be painted

		//create the quad
		Mesh aux;
		aux.createQuad(screenPosition[i].x, screenPosition[i].y, sizes[i].x * factorOfDistortion, sizes[i].y, false);
		quads.push_back(aux);
	}
}

//upload the variables to the quad
void EntityGuiText::setVariables() {

	float width = Game::instance->window_width;
	float height = Game::instance->window_height;

	//the position of the icons in the texture
	icons.push_back(Vector4(0, 0, 1, 1));
	icons.push_back(Vector4(0, 0, 0.5, 0.25));
	icons.push_back(Vector4(0, 0.25, 1, 0.25));
	icons.push_back(Vector4(0, 0.5, 1, 0.5));
	
	//the position of the icons on the screen
	screenPosition.push_back(Vector2(width / 2, height / 2));
	screenPosition.push_back(Vector2(width / 2, 525));
	screenPosition.push_back(Vector2(width / 2, 130));
	screenPosition.push_back(Vector2(width / 2, 350));

	//size of the icon to be painted
	sizes.push_back(Vector2(width, height));
	sizes.push_back(Vector2(200, 100));
	sizes.push_back(Vector2(500, 150));
	sizes.push_back(Vector2(550, 275));
}

//update the quads
void EntityGuiText::update(float elapsed_time) {
	float width = Game::instance->window_width;
	float height = Game::instance->window_height;

	sizes[0] = Vector2(width, height);
	screenPosition[0] = Vector2(width / 2, height / 2);
	screenPosition[1] = Vector2(Vector2(width / 2, 525));
	screenPosition[2] = Vector2(width / 2, 130);
	screenPosition[3] = Vector2(width / 2, 340);
	isOnMouse();

	EntityGui::update(elapsed_time);
}

//return true if the mouse is on the button
bool EntityGuiText::isOnMouse() {

	Vector2 mouse = Input::mouse_position;

	//check if the mouse is on the button (pos 1)
	if (mouse.x > screenPosition[1].x - sizes[1].x / 2 && mouse.x < screenPosition[1].x + sizes[1].x / 2 &&
		mouse.y > screenPosition[1].y - sizes[1].y / 2 && mouse.y < screenPosition[1].y + sizes[1].y / 2) {
		color[1] = Vector4(0.7, 0.7, 0.7, 1);
		return true;
	}
	else color[1] = Vector4(1, 1, 1, 0.9);
	return false;
}