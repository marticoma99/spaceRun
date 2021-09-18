#ifndef ENTITY_GUI_IN_GAME
#define ENTITY_GUI_IN_GAME

#include "entityGui.h"

//spacific gui for the game stage
class EntityGuiInGame : public EntityGui {
public:

	//create the quads
	EntityGuiInGame();
	//set the specific properties of the quads
	void setVariables();
	//update the quads
	void update(float elapsed_time);
};

#endif
