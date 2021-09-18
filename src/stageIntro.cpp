#include "stageIntro.h"
#include "input.h"

StageIntro::StageIntro() : Stage::Stage("introStage") {
	guiText = new EntityGuiText("data/gui/intro.png");
}

//init the stage
void StageIntro::atEnter(int level) {
	Sound::PlaySingle("data/sounds/menu.wav", 0.5);
}

void StageIntro::render() {
	//call the gui render
	guiText->render();
}

void StageIntro::update(double elapsed_time) {
	//call the gui update
	guiText->update(elapsed_time);
}

//for clicking the buttons
void StageIntro::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		//do the action
		if (guiText->isOnMouse()) {
			Sound::Play("data/sounds/click.wav", 1, 1);
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
		}
	}
}