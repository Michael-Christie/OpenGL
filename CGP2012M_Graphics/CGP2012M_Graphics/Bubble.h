#pragma once
#include <GL/glew.h>
#include "TextureClass.h"
#include <array>
#include <cmath>
#include "Model.h"
#include "ModelLoaderClass.h"
#include "ShaderClass.h"

class Bubble
{
public:
	Model model;
	ModelImport loader;

	//bool visable = true;
	////set up vertex buffer object
	//GLuint VBO;
	////set up vertex array object
	//GLuint VAO;
	////set up index buffer object
	//GLuint EBO;
	////set up texture
	Texture tex;
	////shaders
	//Shader vSh, fSh;
	//GLuint shaderProgram;
	////position
	float x, y;
	float xSpeed = -0.00f;
	float ySpeed = -0.01f;


public:
	Bubble() {
		loader.LoadOBJ2("..//..//Assets//Models//Meteor.obj", model.vertices, model.texCoords, model.normals, model.indices);
		tex.load("..//..//Assets//Textures//Meteor.png");
		xSpeed = (rand() % 2 == 1) ? -0.01f : 0.01f;
		ySpeed = (rand() % 2 == 1) ? -0.01f : 0.01f;


	}

	~Bubble() {

	}

	void setBuffers() {
		model.setBuffers(false);
		tex.setBuffers();
	}

	void render() {

		model.render();
	}

};
