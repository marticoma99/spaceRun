#ifndef STAGE_MENU_H
#define STAGE_MENU_H

#include "stage.h"
#include "entityGuiMenu.h"

//stage for the menu
class StageMenu : public Stage {
public:

	EntityGuiMenu* guiMenu; //gui for the menu

	StageMenu();
	//init the stage
	void atEnter(int level);
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif