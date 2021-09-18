#ifndef STAGE_GAME_H
#define STAGE_GAME_H

#include "stage.h"
#include "scene.h"

//Stage for a game
class StageGame : public Stage {
public:
	Scene* scene;
	int level = 0; //level playing

	StageGame(int level);
	//init the stage for the current level
	void atEnter(int _level);
	//render the current level
	void render();
	//draw all the needed text
	void drawTextGui();
	//update the current level
	void update(double elapsed_time);
	//called when leave a level
	void atLeave();
};

#endif
