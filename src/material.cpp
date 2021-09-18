#include "material.h"

Material::Material(Vector3 _ambient, Vector3 _diffuse, Vector3 _specular, float _shininess) {
	ambient = _ambient;
	diffuse = _diffuse;
	specular = _specular;
	shininess = _shininess;
}
Material::Material() {
	//dafault material parameters
	ambient = Vector3(1, 1, 1);
	diffuse = Vector3(1, 1, 1);
	specular = Vector3(0.5, 0.5, 0.5);
	shininess = 30.0f;
}

//upload the parameters to the shader
void Material::uploadShader(Shader* s) {
	s->setVector3("ambient", ambient);
	s->setVector3("diffuse", diffuse);
	s->setVector3("specular", specular);
	s->setUniform("shininess", shininess);
}