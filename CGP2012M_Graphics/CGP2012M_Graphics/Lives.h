#pragma once
#include <GL/glew.h>
#include <array>
#include <ctime>
#include "ShaderClass.h"
#include "TextureClass.h"

class Lives
{
public:
	Shader vSh1, fSh1;
	GLuint shaderProgram;
	Texture tex;


	Lives()
	{
		vSh1.shaderFileName("..//..//Assets//Shaders//shader_normal.vert");
		fSh1.shaderFileName("..//..//Assets//Shaders//shader_normal.frag");

		vSh1.getShader(1);
		fSh1.getShader(2);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vSh1.shaderID);
		glAttachShader(shaderProgram, fSh1.shaderID);
		glLinkProgram(shaderProgram);

		glDeleteShader(vSh1.shaderID);
		glDeleteShader(fSh1.shaderID);

		tex.load("..//..//Assets//Textures//Lives.png");

	}

	GLfloat vertices[48] = {
		// tri one
		  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		  0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		  0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,

		  //tri two
		  1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
		 0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		  1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,    1.0f, 1.0f,


	};



	GLuint VBO;
	GLuint VAO;

	void setBuffers()
	{
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);

		tex.setBuffers();
	}

	void render()
	{
		glUseProgram(shaderProgram);
		glBindTexture(GL_TEXTURE_2D, tex.texture);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
};

