#include "stageTutorial.h"
#include "input.h"
#include "game.h"

StageTutorial::StageTutorial() : Stage::Stage("tutorialStage") {
	guiText = new EntityGuiText("data/gui/tutorial/tutorial1.png");
}

void StageTutorial::atEnter(int level) {
	tutNum = 1;
}

void StageTutorial::render() {
	//set the image of the actual tutorial
	std::string name = "data/gui/tutorial/tutorial" + std::to_string(tutNum) + ".png";
	guiText->texture = Texture::Get(name.c_str());

	//call the gui render
	guiText->render();
}

void StageTutorial::update(double elapsed_time) {
	//call the gui update
	guiText->update(elapsed_time);
}

//load all needed meshes
void StageTutorial::onMouseButtonDown() {
	//is left button pressed
	if ((Input::mouse_state & SDL_BUTTON_LEFT))
	{
		if (guiText->isOnMouse()) {
			Sound::Play("data/sounds/click.wav", 1, 1);
			if (tutNum == tutTotal) {
				stages["stageGame0"]->atEnter(0);
				Stage::changeStage("stageGame0");
			}
			else tutNum++;
		}
	}
}