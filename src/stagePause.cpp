#include "stagePause.h"
#include "input.h"
#include "game.h"
#include "scene.h"

StagePause::StagePause() : Stage::Stage("pauseStage") {
	guiMenu = new EntityGuiMenu("data/gui/pause.png");
}

//init the stage
void StagePause::atEnter(int _level) {
	level = _level;
	Sound::PlaySingle("data/sounds/menu.wav", 0.5);
}

void StagePause::render() {
	//call the gui render
	guiMenu->render();
}

void StagePause::update(double elapsed_time) {
	//call the gui update
	guiMenu->update(elapsed_time);
}

//load all needed meshes
void StagePause::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		//search which button has been presed and do the corresponding action
		int pos = guiMenu->isOnMouse();
		if (pos == 1) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			Scene::instance->saveGameInfo();
		}
		if (pos == 2) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
		}
		if (pos == 3) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			Stage::changeStage("stageGame" + std::to_string(level));
		}
	}
}