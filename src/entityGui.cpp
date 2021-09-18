#include "entityGui.h"
#include "scene.h"
#include "game.h"

EntityGui::EntityGui() {
	shader = NULL;
	texture = NULL;
}

//render the quads in the screen
void EntityGui::render() {

	//ortographic camera for not having distortion
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);

	glDisable(GL_DEPTH_TEST); //disable de z-bufer
	glDisable(GL_CULL_FACE); //disable the cull face
	glEnable(GL_BLEND); //set the blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->enable();

	//render all quads
	for (int i = 0; i < numberOfquads; i++) {
		if(haveBack && i == 0) shader->setTexture("u_texture", textureBack, 0);
		else shader->setTexture("u_texture", texture, 0);
		shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
		shader->setUniform("partToPaint", cam2D.viewprojection_matrix);
		shader->setUniform("partToPaint", partToPaint[i]);
		shader->setUniform("u_color", color[i]);
		shader->setUniform("range", icons[i]);

		quads[i].render(GL_TRIANGLES);
	}
	glEnable(GL_DEPTH_TEST); //enable the z-bufer again
}

//create the quads for every update with the current parameters
void EntityGui::update(float elapsed_time) {
	//create the quads every
	for (int i = 0; i < numberOfquads; i++)
		quads[i].createQuad(screenPosition[i].x, screenPosition[i].y, sizes[i].x * factorOfDistortion, sizes[i].y, false);
}