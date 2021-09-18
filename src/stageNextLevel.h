#ifndef STAGE_NEXT_H
#define STAGE_NEXT_H

#include "stage.h"
#include "entityGuiMenu.h"

//stage when finishng a level
class StageNext : public Stage {
public:

	EntityGuiMenu* guiMenu;
	int level = 0; //level where the stage was called

	StageNext();
	//init the stage
	void atEnter(int _level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif