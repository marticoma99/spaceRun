#ifndef STAGE_WIN_H
#define STAGE_WIN_H

#include "stage.h"
#include "entityGuiMenu.h"

//stage for when the player win the game
class StageWin : public Stage {
public:

	EntityGuiMenu* guiMenu; //gui for this stage

	StageWin();
	//init the stage
	void atEnter(int level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif