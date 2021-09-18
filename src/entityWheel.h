#ifndef ENTITY_WHEEL_H
#define ENTITY_WHEEL_H

#include "entityMesh.h"

//Class that represents the wheels of the player
class EntityWheel : public EntityMesh {
public:

	enum { leftWheel, rightWheel }; //posible types of wheels
	int rotateAngle[2] = {-180,180}; //angle of rotation for the wheels
	int rotationPosition[2] = { -180, 0 }; //rotate the left wheel to be the invsers of the right wheel
	int wheelPos = -1; //right or left wheel

	EntityWheel(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent, int pos);

	//rotate the wheels if the player is moving
	void update(float elapsed_time);
};
#endif