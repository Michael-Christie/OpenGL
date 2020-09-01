#pragma once
#include <GL/glew.h>
#include "TextureClass.h"
#include <array>
#include <cmath>
#include "Model.h"
#include "ModelLoaderClass.h"
#include "ShaderClass.h"

class Player
{
public:


	Model model;
	ModelImport loader;
	float x = 0;
	float y = 0;
	Texture tex;
	Player() {
		loader.LoadOBJ2("..//..//Assets//Models//Player.obj", model.vertices, model.texCoords, model.normals, model.indices);
		tex.load("..//..//Assets//Textures//ship.png");


	}

	void setBuffers() {
		model.setBuffers(true);
		tex.setBuffers();
	}

	void render() {

		model.render();
	}

	glm::vec3 position() {
		return glm::vec3(x, y, 0.0f);
	}

};