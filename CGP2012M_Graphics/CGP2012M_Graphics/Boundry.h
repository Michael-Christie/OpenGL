#pragma once
#include <GL/glew.h>
#include "TextureClass.h"
#include <array>
#include <cmath>
#include "Model.h"
#include "ModelLoaderClass.h"
#include "ShaderClass.h"

class Boundry
{
public:


	Model model;
	ModelImport loader;

	Texture tex;
	Boundry() {

		loader.LoadOBJ2("..//..//Assets//Models//Boundry.obj", model.vertices, model.texCoords, model.normals, model.indices);
		tex.load("..//..//Assets//Textures//bricks.png");

	}

	void setBuffers() {
		model.setBuffers(true);
		tex.setBuffers();
	}

	void render() {

		model.render();
	}

};
