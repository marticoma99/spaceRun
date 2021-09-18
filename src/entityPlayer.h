
#ifndef ENTITY_PLAYER_H
#define ENTITY_PLAYER_H
#include "entityMesh.h"
#include "light.h"

//particle for player movement
struct particle {
	Vector3 vel;
	float angle;
	float velAngular;
	int rotationSpeed;
	float friction;
};

//Class that represents the body of the player
class EntityPlayer : public EntityMesh {
public:
	
	int camaraPos = 2; //set the camare position back to the player
	particle playerParticle; //phisics for the player

	EntityPlayer(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent);

	//render the player and call the render for the wheels
	void render();
	//update the player and call the update for the wheels
	void update(float elapsed_time);
	//check the input do an action if needed
	void checkInput();
	//check and update the player movement
	void checkMove(float elapsed_time);
	//update the player position and angle
	void updateMove(float elapsed_time, Vector3 velocity, int yaw, bool playerMove);
	//update the position with new new pos if the position is valid
	void updatePosition(Vector3 futurePos, float futureAngle, float elapsed_time);
	//test if is colliding with any object of the scene 
	void testObjects(float elapsed_time);
	//for collactable elements if is colliding remove the element
	int checkCollectable(int* pos);
	//set a bridge in the radioactive river if its possible
	void setBridge();
	//activate a laver if its possible
	void activateLever();
	//show and unshow the levers order
	void showLevers();
	//return true if the player have finished the level
	void checkWinState();
};

#endif
