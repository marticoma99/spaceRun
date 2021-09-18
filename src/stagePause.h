#ifndef STAGE_PAUSE_H
#define STAGE_PAUSE_H

#include "stage.h"
#include "entityGuiMenu.h"

//stage for when the player pause a game
class StagePause : public Stage {
public:

	EntityGuiMenu* guiMenu;
	int level = 0; //level where the stage was called

	StagePause();
	//init the stage
	void atEnter(int _level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif