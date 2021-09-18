#include "light.h"

Light::Light(Vector3 _position, Vector3 _diffuse_color, Vector3 _specular_color, Vector3 _ambientLight) {
	position = _position;
	diffuse_color = _diffuse_color;
	specular_color = _specular_color;
	ambientLight = _ambientLight;
}

Light::Light() {
	//default parameters of the light
	position = Vector3(25, 500, 0);
	diffuse_color = Vector3(0.4f, 0.4f, 0.45f);
	specular_color = Vector3(0.8f, 0.8f, 0.85f);
	ambientLight = Vector3(0.6f, 0.6f, 0.65f);
}

//uploads the parameters of the light to the shader
void Light::uploadShader(Shader* s) {
	s->setVector3("position", position);
	s->setVector3("diffuse_color", diffuse_color);
	s->setVector3("specular_color", specular_color);
	s->setVector3("ambientLight", ambientLight);
}