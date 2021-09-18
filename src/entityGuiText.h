#ifndef ENTITY_GUI_TEXT
#define ENTITY_GUI_TEXT

#include "entityGui.h"

//spacific gui for the menus with text
class EntityGuiText : public EntityGui {
public:

	//create the quads
	EntityGuiText(const char* name);
	//set the specific properties of the quads
	void setVariables();
	//update the quads
	void update(float elapsed_time);
	//return true if the mouse is on the button
	bool isOnMouse();
};

#endif