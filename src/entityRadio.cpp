#include "entityRadio.h"

EntityRadio::EntityRadio(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent) :
	EntityMesh(mesh, texture, shader, model, parent) {
	//the mesh and texture for the bridge
	bridgeM = Mesh::Get("data/objects/bridge.obj");
	bridgeT = Texture::Get("data/objects/bridge.tga");
}
void EntityRadio::setBridge() {
	this->mesh = bridgeM;
	this->texture = bridgeT;
	haveBridge = true;
}