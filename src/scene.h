#ifndef SCENE_H
#define SCENE_H

#include "entity.h"
#include "entityPlayer.h"
#include "entityWheel.h"
#include "entityMesh.h"
#include "entityEnemy.h"
#include "world.h"
#include "objects.h"
#include "entityGuiInGame.h"

//size of the levers sequence
#define SIZEOFSEQUENCE 4

//the game info to be stored when save a game
struct sGameInfo{
	//Stores player level
	int levelSave;
};

//represents the dificulty f a level
struct Difficulty {
	//Default values of damage and healness
	float ENEMYDAMAGE = 5;
	float RADIOACTIVEDAMAGE = 1000;
	float FUELQUANTITY = 35;
	float FUELLOSE = 1;
	float LIFEGAIN = 1;
	float TIMEBETWEENSHOTS = 0.5f;
	int PLAYERDAMAGE = 35;
};

//reprsesnts all the player info
struct playerPose {
	//meshes and textures for the player and wheels
	Mesh* mesh = NULL;
	Texture* texture = NULL;
	Mesh* wheelM = NULL;
	Texture* wheelT = NULL;

	//Entities for the player body and wheels
	EntityPlayer* player = NULL;
	EntityWheel* wheel1 = NULL;
	EntityWheel* wheel2 = NULL;
};

//Class that contain all information of a game
class Scene {
public:
	static Scene* instance;
	//actual playing level
	int level;

	//List of enemies and projectiles shoted
	std::vector<EntityMesh*>  sEnemy1;
	std::vector<EntityMesh*>  sEnemy2;
	std::vector<EntityMesh*> shotsEnemy;
	std::vector<EntityMesh*> shotsPlayer;

	//Map, use for maping a number with the sequence objects (i == 4 || i == 5 || i == 6 || i == 7)
	std::map<int, int> sequenceMaping;
	std::string sequences =""; //valid sequence
	std::string vSequence = ""; //player add value to construct sequence, use to check if player sequece = sequences

	//Root entity that contain all the other entities
	Entity* root = NULL;

	//world map
	World* world = NULL;
	Objects* objects = NULL;

	//general light, material and shader general
	Material* material = new Material();
	Light* light = new Light();
	Shader* shader = NULL;

	playerPose playerCell; //all the properties of the player
	Vector3 doorPos; //position of the closed door
	Vector3 playerPos; //position of the player

	Difficulty difficulty;

	//texture and mesh for the projectiles
	Mesh* projectileM = NULL;
	Texture* projectileT = NULL;

	int levelsCompleted; //Stores levels completed
	bool win = false; //Determind whether player complete level won it

	//Array of sequence
	int sequence[SIZEOFSEQUENCE] = { 1,2,3,4 };

	//dafault values of player
	float playerFuel = 100;
	float playerHealth = 100;
	float playerVelocityLevel = 0.1f;
	float playerVelocity = 0.1f;

	//objects that player can have
	int numberOfBridge = 0;
	bool haveMap = false;

	//gui of the fuel and helth
	EntityGuiInGame* gui;
	//image that can be shown in screen
	EntityGui* image2show = NULL;
	bool isShowingImage = false;
	float time2show;
	float initTime;

	Scene();
	//set all the objects in the scene
	void init(int _level);
	//set the difficulty of a level
	void setDifficulty();
	//init all the variables of a level
	void initVar();
	//set the player properties
	void setPlayer();

	//update the player life and fuel
	void setLife(float life);
	void setFuel(float fuel);

	//show and unshow an image 
	void showImage(const char* name, Vector2 screenPos, Vector2 size, float _time2show);
	void unSowImage();

	//init the map sequence
	void mapSequenceTo(int sequence[SIZEOFSEQUENCE]);
	//update the sequence and check if is valid
	bool validSequence(int i);
	//return a string of activated levers
	std::string printLevers();

	//Load game data
	bool loadGameInfo();
	void saveGameInfo();
};

#endif