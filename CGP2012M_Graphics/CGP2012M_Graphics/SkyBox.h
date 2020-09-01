#pragma once
#include <GL/glew.h>
#include <array>
#include "ShaderClass.h"
#include "TextureClass.h"
#include <vector>

class SkyBox
{
public:
	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;


	//shader for the square
	Shader vSh, fSh;
	GLuint shaderProgram;

	std::vector<const GLchar *> faces;

	Texture texture;
	GLuint tex;

	//define vertices for the square
	GLfloat SkyBoxVert[108] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//constructor
	SkyBox()
	{
		//shaders
		vSh.shaderFileName("..//..//Assets//Shaders//shader_skybox.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//shader_skybox.frag");

		vSh.getShader(1);
		fSh.getShader(2);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vSh.shaderID);
		glAttachShader(shaderProgram, fSh.shaderID);
		glLinkProgram(shaderProgram);

		glDeleteShader(vSh.shaderID);
		glDeleteShader(fSh.shaderID);

	};



	void setBuffers()
	{
		//
		//OpenGL buffers
		//set up 1 for the triangle
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);
		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SkyBoxVert), &SkyBoxVert, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); //


		//Unbind the VAO
		glBindVertexArray(0); //

		faces.push_back("..//..//Assets//Textures//Skybox//right.tga");
		faces.push_back("..//..//Assets//Textures//Skybox//left.tga");
		faces.push_back("..//..//Assets//Textures//Skybox//up.tga");
		faces.push_back("..//..//Assets//Textures//Skybox//down.tga");
		faces.push_back("..//..//Assets//Textures//Skybox//back.tga");
		faces.push_back("..//..//Assets//Textures//Skybox//front.tga");

		tex = texture.SkyBoxBuffer(faces);
	}

	void render()
	{
		//draw the square 
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

};
