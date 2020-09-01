#pragma once
#include <GL/glew.h>

#include "SDL.h"
#include "SDL_image.h"

#include <vector>

#include <iostream>

class Texture
{
public:
	SDL_Surface* tex;
	GLuint texture;

	//load textures
	void load(const char* fileName)
	{
		//use SDL image loading function in this case
		tex = IMG_Load(fileName);

		if (tex == NULL)
		{
			std::cout << "Error loading texture: " << fileName << std::endl;
		}
		else
		{
			std::cout << "Success loading " << fileName << std::endl;
		}

	}

	//set buffers 
	void setBuffers()
	{
		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_2D, this->texture);

		//set texture parameters on currently bound texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (tex->format->BytesPerPixel == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_FreeSurface(tex);
	}

	GLuint SkyBoxBuffer(std::vector<const GLchar *> faces) {

		glGenTextures(1, &texture);

		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

		for (GLuint i = 0; i < faces.size(); i++) {
			load(faces[i]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
			SDL_FreeSurface(tex);
		}

		//set texture parameters on currently bound texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


		return texture;
	}
};