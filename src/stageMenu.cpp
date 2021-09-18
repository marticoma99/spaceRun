#include "stageMenu.h"
#include "input.h"
#include "game.h"

StageMenu::StageMenu() : Stage::Stage("menuStage") {
	guiMenu = new EntityGuiMenu("data/gui/menu.png");
}

//init the stage
void StageMenu::atEnter(int level) {
	Sound::PlaySingle("data/sounds/menu.wav", 0.5);
}

void StageMenu::render() {
	//call the gui render
	guiMenu->render();
}

void StageMenu::update(double elapsed_time) {
	//call the gui update
	guiMenu->update(elapsed_time);
}

//for clicking the buttons
void StageMenu::onMouseButtonDown() {
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
			Stage::changeStage("controlsStage");
		}
		if (pos == 1) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["levelStage"]->atEnter(0);
			Stage::changeStage("levelStage");
		}
	}
}