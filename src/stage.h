#ifndef STAGE_H
#define STAGE_H

#include "framework.h"
#include "utils.h"
#include <map>
#include "sound.h"

//the basic class for the stages
class Stage {
public:

	static std::map<std::string, Stage*> stages;//list of all possible stages
	static Stage* currentStage;//current used stage

	//constructor, save the stage into the map
	Stage(std::string name) { stages[name] = this; }
	//change the current stage
	static void changeStage(std::string name) { currentStage = stages[name]; }
	//executed when enter no the stage
	virtual void atEnter(int level) {};
	//render for all stages
	virtual void render() {};
	//update for all stages
	virtual void update(double elapsed_time) {};
	//executed when leave no the stage
	virtual void atLeave() {};
	//if some mouse buttom have been pressed
	virtual void onMouseButtonDown() {};
};

#endif