#ifndef STAGE_CONTROLS_H
#define STAGE_CONTROLS_H

#include "stage.h"
#include "entityGuiText.h"

//stage showing the game controls
class StageControls : public Stage {
public:
	EntityGuiText* guiText;

	StageControls();
	void render();
	void update(double elapsed_time);
	//called when mouse clicked
	void onMouseButtonDown();
};

#endif