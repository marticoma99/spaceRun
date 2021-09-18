#ifndef LIGHT_H
#define LIGHT_H

#include "framework.h"
#include "shader.h"

//Represents the light
class Light {
public:
	//position of the light source
	Vector3 position;
	//the parameters for phong shader
	Vector3 diffuse_color;
	Vector3 specular_color;
	Vector3 ambientLight;

	Light(Vector3 position, Vector3 diffuse_color, Vector3 specular_color, Vector3 ambientLight);
	Light();
	//uploads the parameters to the shader
	void uploadShader(Shader* s);

};

#endif