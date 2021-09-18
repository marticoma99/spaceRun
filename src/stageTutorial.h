#ifndef STAGE_TUTORIAL_H
#define STAGE_TUTORIAL_H

#include "stage.h"
#include "entityGuiText.h"

//stage for the tutorials
class StageTutorial : public Stage {
public:

	EntityGuiText* guiText;
	int tutNum = 1;//which tutorial is in screen
	int tutTotal = 10;//total number of tutorials

	StageTutorial();
	//init the stage
	void atEnter(int level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif