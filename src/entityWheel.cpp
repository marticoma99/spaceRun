#include "entityWheel.h"
#include "input.h"

EntityWheel::EntityWheel(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent, int pos) :
EntityMesh(mesh, texture, shader, model, parent) {

	//defines the right and left wheel
	wheelPos = pos;

	//set the rotation of the wheel (the left one have to be rotated to be the inverse of the right one)
	this->model.rotate(rotationPosition[pos] * DEG2RAD, Vector3(0, 1, 0));
	//set the posotion next to the robot body
	this->model.translate(0.21, 0.21, 0);
}

//rotate the wheels if the player is moving
void EntityWheel::update(float elapsed_time) {

	//set the animation of rotation for the wheel
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) model.rotate(rotateAngle[wheelPos] * elapsed_time * DEG2RAD, Vector3(1, 0, 0));
	else if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) model.rotate(-rotateAngle[wheelPos] * elapsed_time * DEG2RAD, Vector3(1, 0, 0));
	else if ((Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT))&& wheelPos == leftWheel) model.rotate(rotateAngle[wheelPos] * elapsed_time * DEG2RAD, Vector3(1, 0, 0));
	else if ((Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) && wheelPos == rightWheel) model.rotate(rotateAngle[wheelPos] * elapsed_time * DEG2RAD, Vector3(1, 0, 0));
}