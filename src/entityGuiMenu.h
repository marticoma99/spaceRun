#ifndef ENTITY_GUI_MENU
#define ENTITY_GUI_MENU

#include "entityGui.h"

//spacific gui for the menus
class EntityGuiMenu : public EntityGui {
public:

	//create the quads
	EntityGuiMenu(const char* name);
	//set the specific properties of the quads
	void setVariables();
	//update the quads
	void update(float elapsed_time);
	//check in which button is the mouse
	int isOnMouse();
};

#endif