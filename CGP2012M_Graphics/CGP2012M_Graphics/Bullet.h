#pragma once
#pragma once
#include <GL/glew.h>
#include "TextureClass.h"
#include <array>
#include <cmath>
#include "Model.h"
#include "ModelLoaderClass.h"
#include "ShaderClass.h"

class Bullet
{
public:


	Model model;
	ModelImport loader;
	float x = 5;
	float y = 5;
	Texture tex;
	Bullet() {

		loader.LoadOBJ2("..//..//Assets//Models//Bullet.obj", model.vertices, model.texCoords, model.normals, model.indices);
		tex.load("..//..//Assets//Textures//Bullet.png");

	}

	void setBuffers() {
		model.setBuffers(true);
		tex.setBuffers();
	}

	void render() {

		model.render();
	}

};