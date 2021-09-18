#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework.h"
#include "shader.h"

//Represents the material properties
class Material {
public:
	//all the properties for the phong shader
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;

	Material(Vector3 _ambient, Vector3 _diffuse, Vector3 _specular, float _shininess);
	Material();
	//upload the material properties to the shader
	void uploadShader(Shader* s);
};

#endif
