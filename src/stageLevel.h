#ifndef STAGE_LEVEL_H
#define STAGE_LEVEL_H

#include "stage.h"
#include "entityGuiMenu.h"

//stage for the levels menu
class StageLevel : public Stage {
public:
	
	EntityGuiMenu* guiMenu;//gui for the levels menu
	int unlockedLevels = 0; //unlocked levels

	StageLevel();
	//init the stage
	void atEnter(int level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif