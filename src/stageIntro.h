#ifndef STAGE_INTRO_H
#define STAGE_INTRO_H

#include "stage.h"
#include "entityGuiText.h"

//stage for intro
class StageIntro : public Stage {
public:
	EntityGuiText* guiText;

	StageIntro();
	//init the stage
	void atEnter(int level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif