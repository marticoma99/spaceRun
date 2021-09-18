#include "stageLevelFail.h"
#include "input.h"
#include "game.h"

StageFail::StageFail() : Stage::Stage("failStage") {
	guiMenu = new EntityGuiMenu("data/gui/lvlFailed.png");
}

//init the stage
void StageFail::atEnter(int _level) {
	level = _level;
	Sound::PlaySingle("data/sounds/menu.wav", 0.5);
}

void StageFail::render() {
	//call the gui render
	guiMenu->render();
}

void StageFail::update(double elapsed_time) {
	//call the gui update
	guiMenu->update(elapsed_time);
}

//for clicking the buttons
void StageFail::onMouseButtonDown() {
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
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["stageGame" + std::to_string(level)]->atEnter(level);
			Stage::changeStage("stageGame" + std::to_string(level));
		}
		if (pos == 1) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
		}
	}
}
