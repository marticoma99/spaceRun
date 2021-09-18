#include "stageControls.h"
#include "input.h"
#include "game.h"

StageControls::StageControls() : Stage::Stage("controlsStage") {
	guiText = new EntityGuiText("data/gui/controls.png");
}

void StageControls::render() {
	//call the gui render
	guiText->render();
}

void StageControls::update(double elapsed_time) {
	//call the gui update
	guiText->update(elapsed_time);
}

void StageControls::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		//do the needed action
		if (guiText->isOnMouse()) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
		}
	}
}