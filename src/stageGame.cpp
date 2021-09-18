#include "stageGame.h"
#include "input.h"
#include "game.h"
#include "sound.h"

StageGame::StageGame(int _level) : Stage::Stage("stageGame" + std::to_string(_level)){

	level = _level;
	//new scene with all the entities of the game
	if (Scene::instance == NULL) scene = new Scene();
	else scene = Scene::instance;
}

//init the scene for the current level
void StageGame::atEnter(int _level) {
	
	Scene::instance->init(_level);//init the class scene with the current level
	int height = Game::instance->window_height;
	int width = Game::instance->window_width;

	//show level name
	std::string name = "data/gui/floor" + std::to_string(_level) + ".png";
	Scene::instance->showImage(name.c_str(), Vector2(width / 2, height / 2), Vector2(600, 600), 3);

	//load all needed sounds
	Sound::Get("data/sounds/prova.wav", 1);
	Sound::PlaySingle("data/sounds/main.wav", 0.5);
	Sound::StopAudio("data/sounds/menu.wav");

	//load all needed textures
	Texture::Get("data/gui/levers.png");
	Texture::Get("data/gui/correct.png");
	Texture::Get("data/gui/incorrect.png");
}

//render the game scene and gui
void StageGame::render() {
	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//set the clear color (the background color)
	glClearColor(0, 0, 0.1, 1.0);

	//call the scene render
	scene->root->render();

	//draw all gui text
	drawTextGui();
}

//draw all text of the gui
void StageGame::drawTextGui() {
	//render the FPS, Draw Calls, etc
	//drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//draw the activated lvers
	drawText(30, Game::instance->window_height - 80, "Activated levers:", Vector3(1, 0.4, 0.27), 3);
	drawText(30, Game::instance->window_height - 50, Scene::instance->printLevers(), Vector3(1, 0.4, 0.27), 3);

	//draw the collected bridges
	std::string bridges = "Bridges x" + std::to_string(Scene::instance->numberOfBridge);
	drawText(Game::instance->window_width - 200, 78, bridges.c_str(), Vector3(1, 0.4, 0.27), 3);

	//draw the collected levers paper
	std::string leverOrder = "Levers paper x";
	if (Scene::instance->haveMap) leverOrder += "1";
	else leverOrder += "0";
	drawText(Game::instance->window_width - 297, 40, leverOrder.c_str(), Vector3(1, 0.4, 0.27), 3);
}

//update the game stage
void StageGame::update(double elapsed_time) {
	//update the scene
	scene->root->update(elapsed_time);

	//check if the player have fuel and life
	if (Scene::instance->playerHealth <= 1 || Scene::instance->playerFuel <= 0) {
		Sound::Play("data/sounds/die.wav", 0.5, 1);
		atLeave();
		stages["failStage"]->atEnter(level);
		Stage::changeStage("failStage");
	}
	//check if the player have completed the game
	if (Scene::instance->win && Scene::instance->level == 1) {
		Sound::Play("data/sounds/win.wav", 0.5, 1);
		atLeave();
		stages["winStage"]->atEnter(level);
		Stage::changeStage("winStage");
	}
	//check if the player have win a level
	else if (Scene::instance->win) {
		Sound::Play("data/sounds/win.wav", 0.5, 1);
		atLeave();
		stages["nextStage"]->atEnter(level);
		Stage::changeStage("nextStage");
	}
	//pause the game
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
		Sound::Play("data/sounds/click.wav", 2, 1);
		atLeave();
		stages["pauseStage"]->atEnter(level);
		Stage::changeStage("pauseStage");
	}

	//unshow the image if its time
	if (Scene::instance->isShowingImage) {
		if (Scene::instance->initTime + Scene::instance->time2show < Game::instance->time) Scene::instance->unSowImage();
	}
}

//called when leave the game
void  StageGame::atLeave() {
	//stop all the possible audios
	Sound::StopAudio("data/sounds/move.wav");
	Sound::StopAudio("data/sounds/main.wav");
	Sound::StopAudio("data/sounds/forceField.wav");
	Scene::instance->saveGameInfo();
}