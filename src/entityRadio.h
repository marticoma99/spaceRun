#ifndef ENTITY_RADIO_H
#define ENTITY_RADIO_H

#include "entityMesh.h"

class EntityRadio : public EntityMesh {
public:
	bool haveBridge = false;
	Mesh* bridgeM = NULL;
	Texture* bridgeT = NULL;

	EntityRadio(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model, Entity* parent);
	void setBridge();
};

#endif