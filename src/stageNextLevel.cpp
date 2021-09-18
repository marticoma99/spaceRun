#include "stageNextLevel.h"
#include "input.h"
#include "game.h"
#include "scene.h"

StageNext::StageNext() : Stage::Stage("nextStage") {
	guiMenu = new EntityGuiMenu("data/gui/lvlCompleted.png");
}

//init the stage
void StageNext::atEnter(int _level) {
	Scene::instance->levelsCompleted = _level + 1;
	Scene::instance->saveGameInfo();
	level = _level;
	Sound::PlaySingle("data/sounds/menu.wav", 0.5);
}

void StageNext::render() {
	//call the gui render
	guiMenu->render();
}

void StageNext::update(double elapsed_time) {
	//call the gui update
	guiMenu->update(elapsed_time);
}

//for clicking the buttons
void StageNext::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		//search which button has been presed and do the corresponding action
		int pos = guiMenu->isOnMouse();
		if (pos == 3) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			Game::instance->must_exit = true;
		}
		if (pos == 2) {
			atLeave();
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["stageGame" + std::to_string(level + 1)]->atEnter(level + 1);
			Stage::changeStage("stageGame" + std::to_string(level + 1));
		}
		if (pos == 1) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
		}
	}
}
