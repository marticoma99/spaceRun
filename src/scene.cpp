#include "scene.h"
#include <cassert>
#include "game.h"
#include "sound.h"


Scene* Scene::instance = NULL;

Scene::Scene() {
	//only one instance of type scene
	if(instance == NULL) instance = this;
	loadGameInfo();
}

//init the scene with the current level
void Scene::init(int _level) {

	//init the scene with the current level
	level = _level;

	//general shader
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	//load the texture and mesh for the projectiles
	projectileM = Mesh::Get("data/objects/projectile.obj");
	projectileT = Texture::Get("data/objects/projectile.tga");
	
	initVar(); //init all the variables of the level
	setPlayer(); //set the player entity
	setDifficulty(); //set the level difficulty
	mapSequenceTo(sequence); //Maps sequence to lever
	loadGameInfo(); //Loads max level riched
}

//set the difficulty of a level
void Scene::setDifficulty() {
	//difficulty for level 0
	if (level == 0) {
		this->difficulty.PLAYERDAMAGE = 5;
		this->difficulty.FUELLOSE = 1;
		this->difficulty.PLAYERDAMAGE = 35;
		this->difficulty.LIFEGAIN = 1.2;
	}
	//difficulty for level 1
	if (level == 1) {
		this->difficulty.PLAYERDAMAGE = 7;
		this->difficulty.FUELLOSE = 1.2;
		this->difficulty.PLAYERDAMAGE = 20;
		this->difficulty.LIFEGAIN = 1.35;
	}
}

//init all the variables of a level
void Scene::initVar() {

	//remove the previous data if needed
	if(root != NULL) delete root;
	root = new Entity();
	this->root = root;
	sEnemy1.clear();
	sEnemy2.clear();
	shotsPlayer.clear();
	shotsEnemy.clear();

	//load the world map
	if (world != NULL) delete world;
	if (objects != NULL) delete objects;
	objects = new Objects(root, level);
	world = new World(root, level);

	gui = new EntityGuiInGame();

	playerFuel = 100;
	playerHealth = 100;
	playerVelocity = 0.1f;
	playerVelocityLevel = 0.1f;
	win = false;

	numberOfBridge = 0;
	haveMap = false;
}

//set the player properties
void Scene::setPlayer() {

	Matrix44 model = Matrix44();
	//robot and wheel texture
	playerCell.texture = Texture::Get("data/objects/robot.tga");
	playerCell.wheelT = Texture::Get("data/objects/wheel.tga");

	//robot and wheel mesh
	playerCell.mesh = Mesh::Get("data/objects/robot.obj");
	playerCell.wheelM = Mesh::Get("data/objects/wheel.obj");

	//set the player and wheels
	playerCell.player = new EntityPlayer(playerCell.mesh, playerCell.texture, shader, model, root);
	playerCell.wheel1 = new EntityWheel(playerCell.wheelM, playerCell.wheelT, shader, model, playerCell.player, 1);
	playerCell.wheel2 = new EntityWheel(playerCell.wheelM, playerCell.wheelT, shader, model, playerCell.player, 0);

	//the player is a child of the root entity
	root->addChild(playerCell.player);

	//the wheels are attached to tha player body
	playerCell.player->addChild(playerCell.wheel1);
	playerCell.player->addChild(playerCell.wheel2);
}

//set the player life
void Scene::setLife(float life) {
	this->playerHealth += life;
	if (playerHealth > 100) playerHealth = 100;
	if (playerHealth < 0) playerHealth = 0;
}

//set the player fuel
void Scene::setFuel(float fuel)
{
	this->playerFuel += fuel;
	if (playerFuel > 100) playerFuel = 100;
}

//show an image name _time2show seconds
void Scene::showImage(const char* name, Vector2 screenPos, Vector2 size, float _time2show) {

	//create the gui
	if (isShowingImage) unSowImage();
	if (image2show != NULL) delete image2show;
	image2show = new EntityGui();

	//set the gui properties
	image2show->numberOfquads = 1;
	image2show->shader = Shader::Get("data/shaders/gui.vs", "data/shaders/gui.fs");
	image2show->texture = Texture::Get(name);

	//set the properties of screen
	image2show->icons.push_back(Vector4(0,0,1,1));
	image2show->screenPosition.push_back(screenPos);
	image2show->partToPaint.push_back(4096);
	image2show->sizes.push_back(size);
	image2show->color.push_back(Vector4(1,1,1,1));

	//create the quad
	Mesh aux;
	aux.createQuad(screenPos.x, screenPos.y, size.x, size.y, false);
	image2show->quads.push_back(aux);
	root->addChild(image2show);

	isShowingImage = true;
	time2show = _time2show;
	initTime = Game::instance->time;
}

//unshow the image, only if we where showing an image
void Scene::unSowImage() {
	root->rmvChild(image2show);
	isShowingImage = false;
	delete image2show;
	image2show = NULL;
}

//This function maps a given sequence to the "palanca" (i == 4 || i == 5 || i == 6 || i == 7)
void Scene::mapSequenceTo(int sequence[SIZEOFSEQUENCE])
{
	//Mapping
	sequenceMaping.insert(std::pair<int, int>(4 , sequence[0]));
	sequenceMaping.insert(std::pair<int, int>(5 , sequence[1]));
	sequenceMaping.insert(std::pair<int, int>(6,  sequence[2]));
	sequenceMaping.insert(std::pair<int, int>(7,  sequence[3]));

	//valid sequence
	sequences = "3214";
	//sequence introduced by the player
	vSequence = "";
}

//Check if the squeence find by player is ok
bool Scene::validSequence(int i)
{
	int value = sequenceMaping[i];
	if (value > 0)
	{
		//search if the lever is already in the sequence
		int pos = vSequence.find(std::to_string(value));
		if ( pos == std::string::npos) vSequence += std::to_string(value); //if the lever is not in the sequence add it
		else vSequence.erase(vSequence.begin() + pos); //if the lever is in the sequence remove it

		//if the sequence if completed check if is valid
		if (vSequence.size() == SIZEOFSEQUENCE)
		{
			int height = Game::instance->window_height;
			int width = Game::instance->window_width;
			//if the sequence is correct
			if (vSequence == sequences) {
				showImage("data/gui/correct.png", Vector2(width / 2, height / 2), Vector2(600, 600), 3);
				return true;
			}
			//if the sequence is incorrect
			else {
				Sound::Play("data/sounds/wrong.wav", 0.5, 1);
				showImage("data/gui/incorrect.png", Vector2(width / 2, height / 2), Vector2(600, 600), 3);
				vSequence = "";
			}
		}
	}
	return false;
}

//Save game information
void Scene::saveGameInfo()
{
	sGameInfo game_info;
	//set the level to the game info
	game_info.levelSave = this->levelsCompleted;

	//save to file
	FILE* fp = fopen("savegame.bin", "wb");
	fwrite(&game_info, sizeof(sGameInfo), 1, fp);
	fclose(fp);
}

//load the game info from a file
bool Scene::loadGameInfo()
{
	sGameInfo game_info;

	//load file
	FILE* fp = fopen("savegame.bin", "rb");
	if (fp == NULL) {
		this->levelsCompleted = 0;
		return false; //no savegame found
	}

	fread(&game_info, sizeof(sGameInfo), 1, fp);
	fclose(fp);

	//transfer data from game_info to Game
	this->levelsCompleted = game_info.levelSave;
	return true;
}

//return a string of activated levers
std::string Scene::printLevers() {
	std::string out = "";

	//map the numbers to the lever color
	for (int i = 0; i < vSequence.size(); i++) {
		if (vSequence.at(i) == '1') out += "Purple ";
		if (vSequence.at(i) == '2') out += "Green ";
		if (vSequence.at(i) == '3') out += "Red ";
		if (vSequence.at(i) == '4') out += "Yellow ";
		if (i != vSequence.size() - 1) out += "- ";
	}

	if (vSequence.size() == 0) out = "None";
	return out;
}