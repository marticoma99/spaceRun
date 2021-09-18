#include "stageWin.h"
#include "input.h"
#include "game.h"
#include "scene.h"

StageWin::StageWin() : Stage::Stage("winStage"){
	guiMenu = new EntityGuiMenu("data/gui/win.png");
}

//init the stage
void StageWin::atEnter(int level) {
	Sound::PlaySingle("data/sounds/menu.wav", 0.5);
}

void StageWin::render() {
	//call the gui render
	guiMenu->render();
}

void StageWin::update(double elapsed_time) {
	//call the gui update
	guiMenu->update(elapsed_time);
}

//for clicking the buttons
void StageWin::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		//search which button has been presed and do the corresponding action
		int pos = guiMenu->isOnMouse();
		if (pos == 3) {
			Game::instance->must_exit = true;
			Sound::Play("data/sounds/click.wav", 2, 1);
		}
		if (pos == 2) {
			stages["levelStage"]->atEnter(0);
			Stage::changeStage("levelStage");
			Sound::Play("data/sounds/click.wav", 2, 1);
		}
		if (pos == 1) {
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
			Sound::Play("data/sounds/click.wav", 2, 1);
		} 
	}
}