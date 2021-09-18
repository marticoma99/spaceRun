#ifndef STAGE_FAIL_H
#define STAGE_FAIL_H

#include "stage.h"
#include "entityGuiMenu.h"

//stage for when a player fail a level
class StageFail : public Stage {
public:
	
	EntityGuiMenu* guiMenu; //gui for the stage
	int level = 0; //level where the stage was called

	StageFail();
	//init the stage
	void atEnter(int level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif