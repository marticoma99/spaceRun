#include "stageLevel.h"
#include "input.h"
#include "game.h"
#include "scene.h"

StageLevel::StageLevel() : Stage::Stage("levelStage") {
	guiMenu = new EntityGuiMenu("data/gui/level.png");
}

void StageLevel::render() {
	//call the gui render
	guiMenu->render();
}

//init the stage
void StageLevel::atEnter(int level) {
	//load unlocked levels
	Scene::instance->loadGameInfo();
	unlockedLevels = Scene::instance->levelsCompleted;

	//set in the gui the unlocked levels
	if (unlockedLevels == 0) guiMenu->icons[3] = Vector4(0.5, 0.25, 0.5, 0.25);
	else guiMenu->icons[3] = Vector4(0.5, 0, 0.5, 0.25);
}

void StageLevel::update(double elapsed_time) {
	//call the gui update
	guiMenu->update(elapsed_time);
}

//for clicking the buttons
void StageLevel::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		//search which button has been presed and do the corresponding action
		int pos = guiMenu->isOnMouse();
		if (pos == 3) {
			stages["menuStage"]->atEnter(0);
			Stage::changeStage("menuStage");
			Sound::Play("data/sounds/click.wav", 2, 1);
		}
		if (pos == 1) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["tutorialStage"]->atEnter(0);
			Stage::changeStage("tutorialStage");
		}
		if (pos == 2) {
			if (unlockedLevels) {
			Sound::Play("data/sounds/click.wav", 2, 1);
			stages["stageGame1"]->atEnter(1);
			Stage::changeStage("stageGame1");
			}
			else Sound::Play("data/sounds/wrong.wav", 0.2, 1);
		}
	}
}