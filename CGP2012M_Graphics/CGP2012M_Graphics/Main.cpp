#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
#include "Bubble.h"
#include "Player.h"
#include "Bullet.h"
#include "Square.h"
#include "Camera.h"
#include "Cube.h"
#include "SkyBox.h"
#include "Boundry.h"
#include "Lives.h"
#include "Model.h"
#include "ModelLoaderClass.h"
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H
#ifndef SHADERCLASS_H
#define SHADERCLASS_H

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
//#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#ifndef SDL_H
#define SDL_H
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//***************
//variables
SDL_Event event;
bool keypressed[4];
SDL_Window *win;
bool windowOpen = true;
bool isFullScreen = false;
float bubbleSpeed = -0.001f;
float radius;
int index = 0;
//screen boundaries for collision tests
float rightScreen = 2.15f;
float leftScren = -2.0f;
float topScreen = 1.5f;
float bottonScreen = -1.5f;
//screen centre
float centreX = 0;
float centreY = 0;
//window aspect/width/height
int w;
int h;
float aspect;
int left;
int newwidth;
int newheight;

const int circleActive = 2;
int circlesActive2 = circleActive;
int playerLives = 4;

//transform matrices
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 normalMatrix;

glm::mat4 translate;
glm::mat4 translateBullet;
glm::mat4 LivesPos;
glm::mat4 rotate;
glm::mat4 rotateBullet;
glm::mat4 scale;
glm::mat4 bubbleScale;
glm::mat4 skyboxScale;

glm::mat4 backgroundTranslate;
glm::mat4 backgroundScale;
glm::vec3 b_scaleFactor;
glm::mat4 WallsPos;
glm::mat4 sides;
glm::mat4 modelRotate;
glm::mat4 modelScale;
glm::mat4 modelTranslate;
glm::mat4 UIScale;
glm::mat4 boundryTranslate;

float angle = 0;
float bulletAngle = 0;

std::vector<Bubble> bubbles;

std::vector<glm::mat4> bubblesTranslate;

//create camera
Camera cam;
//loac camera variables
glm::vec3 camPos;
glm::vec3 camTarget;

bool flag = true;

glm::vec3 lightCol1;
glm::vec3 lightPosition1;
glm::vec3 lightCol2;
glm::vec3 lightPosition2;
glm::vec3 lightCol3;
glm::vec3 lightPosition3;

glm::vec3 viewPosition;


float ambientIntensity;

//**************
//function prototypes
Bubble updatePositions(Bubble c, int i);
void handleInput(Player &player, Bullet &bullet);
bool collisions(Player player, Bubble &bubble);
bool collisions(Bullet bullet, Bubble c);

int main(int argc, char *argv[]) {
	//start and initialise SDL
	SDL_Start sdl;
	SDL_GLContext context = sdl.Init();
	win = sdl.win;

	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	aspect = (float)w / (float)h;

	//error class
	GLerror glerr;
	int errorLabel;

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//register debug callback
	if (glDebugMessageCallback)
	{
		std::cout << "Registering OpenGL Debug callback function" << std::endl;
		glDebugMessageCallback(glerr.openglCallbackFunction, &errorLabel);
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			NULL,
			true);
	}

	//*****************************************************
	//OpenGL specific data

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//objects
	//create background square
	//Square background;
	Player player;
	Bullet bullet;
	Lives lives[5];
	SkyBox skybox;
	Boundry boundry;
	//create model

	float randValue, randValue2;
	srand(time(0));
	radius = 0.01f;

	for (int i = 0; i < circlesActive2; i++)
	{
		bubbles.push_back(Bubble());
	}

	errorLabel = 0;

	//*********************

	player.setBuffers();
	bullet.setBuffers();
	skybox.setBuffers();
	for (int i = 0; i < playerLives; i++) {
		lives[i].setBuffers();
	}
	boundry.setBuffers();

	errorLabel = 2;

	for (int q = 0; q < circlesActive2; q++)
	{
		bubbles[q].setBuffers();
	}

	errorLabel = 3;
	//*****************************************
	//set uniform variables
	int transformLocation;
	int modelLocation;
	int viewLocation;
	int projectionLocation;
	int importModelLocation;
	int importViewLocation;
	int importProjectionLocation;
	int backgroundColourLocation;
	int ambientIntensityLocation;
	int modelColourLocation;
	int modelAmbientLocation;
	int lightColLocation;
	int normalMatrixLocation;
	int lightPositionLocation;
	int viewPositionLocation;
	int skyLocation;
	int skyProjection;
	int viewPos;


	GLuint currentTime = 0;
	GLuint lastTime = 0;
	GLuint elapsedTime = 0;

	//lighting for the model
	//Light position setting
	lightPosition1 = glm::vec3(2.0f, 0.0f, 0.5f);
	lightPosition2 = glm::vec3(-2.0f, 0.0f, 0.5f);
	lightPosition3 = glm::vec3(2.0f, 0.0f, 0.5f);

	//light colour setting
	// Candle:  r:1.0 g:0.57 b:0.16
	// 100W bulb: r:1.0 g:0.84 b:0.66
	// direct sunlight: r:1.0 g:1.0 b:0.98
	glm::vec3 lightColour1 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lightColour2 = glm::vec3(1.0f, 00.0f, 0.0f);
	glm::vec3 lightColour3 = glm::vec3(1.0f, 0.84f, 0.66f);


	//light for the background
	//light distance setting
	ambientIntensity = 0.5f;
	lightCol1 = glm::vec3(1.0f, 1.0f, 0.98f);

	projectionMatrix = glm::mat4(1.0f);
	//initialise transform matrices 
	//perspective (3D) projection
	projectionMatrix = glm::perspective(glm::radians(50.0f), (float)w / (float)h, 0.1f, 100.0f);
	//initialise view matrix to '1'
	viewMatrix = glm::mat4(1.0f);

	for (int i = 0; i < circlesActive2; i++)
		bubblesTranslate.push_back(glm::mat4(1.0f));

	translateBullet = glm::mat4(1.0f);
	translate = glm::mat4(1.0f);
	rotate = glm::mat4(1.0f);
	LivesPos = glm::mat4(1.0f);
	WallsPos = glm::mat4(1.0f);
	rotateBullet = glm::mat4(1.0f);
	sides = glm::mat4(1.0f);
	sides = glm::rotate(sides, glm::radians(90.0f), glm::vec3(0, 0, 1));
	bubbleScale = glm::mat4(1.0f);
	skyboxScale = glm::mat4(1.0f);
	LivesPos = glm::mat4(1.0f);
	UIScale = glm::mat4(1.0f);
	boundryTranslate = glm::mat4(1.0f);


	backgroundScale = glm::mat4(1.0f);
	backgroundTranslate = glm::mat4(1.0f);
	modelScale = glm::mat4(1.0f);
	modelRotate = glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);

	//once only scale to background, and translate to centre
	b_scaleFactor = { 60.0f, 50.0f, 1.0f };
	backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.0f, 0.0f, -2.0f));

	//once only scale and translate to model
	modelScale = glm::scale(modelScale, glm::vec3(0.7f, 0.7f, 0.7f));
	modelTranslate = glm::translate(modelTranslate, glm::vec3(0.0f, 0.0f, -1.0f));

	rotate = glm::rotate(rotate, glm::radians(100.0f), glm::vec3(1, 0, 0));
	translate = glm::translate(translate, glm::vec3(centreX, centreY, 0.0f));
	player.x = centreX;
	player.y = centreY;

	for (int i = 0; i < circlesActive2; i++) {
		randValue = (float)rand() / RAND_MAX * (rand() % 2 == 1) ? -2.0f : 2.0f;
		randValue2 = (float)rand() / RAND_MAX * (rand() % 2 == 1) ? -2.0f : 2.0f;;
		bubbles[i].x = centreX + randValue;
		bubbles[i].y = centreY + randValue2;
	}
	bubbleScale = glm::scale(bubbleScale, glm::vec3(0.1f, 0.1f, 0.1f));
	skyboxScale = glm::scale(skyboxScale, glm::vec3(2.0f, 2.0f, 2.0f));
	UIScale = glm::scale(UIScale, glm::vec3(0.25f, 0.25f, 0.25f));


	for (int i = 0; i < circlesActive2; i++)
		bubblesTranslate[i] = glm::translate(bubblesTranslate[i], glm::vec3(bubbles[i].x, bubbles[i].y, 0.0f));

	errorLabel = 4;

	//*****************************
	//'game' loop
	while (windowOpen)
	{
		//*************************
		//****************************
		// OpenGL calls.


		glClearColor(1.0f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//process inputs
		handleInput(player, bullet);
		lightPosition3 = player.position();
		cam.updateCamera();

		//time
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		//update camera matrix
		//camera only moves side to side, formards and backwards (no rotation)
		// set position, target, up direction
		viewMatrix = glm::lookAt(cam.cameraPosition, cam.cameraTarget, cam.cameraUp);
		errorLabel = 5;


#pragma region Background


		////background
		//glUseProgram(background.shaderProgram);
		////set background lighting
		//backgroundColourLocation = glGetUniformLocation(background.shaderProgram, "uLightColour");
		//glProgramUniform3fv(background.shaderProgram, backgroundColourLocation, 1, glm::value_ptr(lightCol));
		////light distance
		//ambientIntensityLocation = glGetUniformLocation(background.shaderProgram, "uAmbientIntensity");
		//glProgramUniform1f(background.shaderProgram, ambientIntensityLocation, ambientIntensity);
		//
		////set background image
		//modelLocation = glGetUniformLocation(background.shaderProgram, "uModel");
		//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(backgroundTranslate*backgroundScale));
		//viewLocation = glGetUniformLocation(background.shaderProgram, "uView");
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		//projectionLocation = glGetUniformLocation(background.shaderProgram, "uProjection");
		//glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		//glBindTexture(GL_TEXTURE_2D, texArray[0].texture);
		//background.render();

#pragma endregion


		int playerHitIndex = -1;
#pragma region Bubbles


		for (int i = 0; i < circlesActive2; i++) {
			bubbles[i] = updatePositions(bubbles[i], i);
			////set .obj model
			glUseProgram(bubbles[i].model.shaderProgram);
			//lighting uniforms
			//get and set light colour and position uniform
			lightColLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "lightCol1");
			glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour1));
			lightPositionLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "lightPos1");
			glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition1));
			//get and set the ligh color and position for light two
			lightColLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "lightCol2");
			glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour2));
			lightPositionLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "lightPos2");
			glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition2));


			//rotation
			modelRotate = glm::rotate(modelRotate, (float)elapsedTime / 2000, glm::vec3(rand() % 100 * 0.2f, 1.0f, 0.0f));
			importModelLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "uModel");
			glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(bubblesTranslate[i] * modelRotate*bubbleScale));
			importViewLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "uView");
			glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));

			viewPos = glGetUniformLocation(bubbles[i].model.shaderProgram, "viewPos");
			glUniform3fv(viewPos, 1, glm::value_ptr(cam.cameraPosition));
			importProjectionLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "uProjection");
			glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			//So we calculate the normal matrix in local space
			normalMatrix = glm::transpose(glm::inverse(bubblesTranslate[i] * modelRotate*bubbleScale * viewMatrix));
			//set the normalMatrix in the shaders
			glUseProgram(bubbles[i].model.shaderProgram);
			normalMatrixLocation = glGetUniformLocation(bubbles[i].model.shaderProgram, "uNormalMatrix");
			glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
			glBindTexture(GL_TEXTURE_2D, bubbles[i].tex.texture);
			bubbles[i].model.render();

			if (collisions(player, bubbles[i])) {
				playerHitIndex = i;
			}
			if (collisions(bullet, bubbles[i])) {
				playerHitIndex = i;
			}
		}
		if (playerHitIndex >= 0) {
			bubbles[playerHitIndex].~Bubble();
			bubbles.erase(bubbles.begin() + playerHitIndex);
			bubblesTranslate.erase(bubblesTranslate.begin() + playerHitIndex);
			circlesActive2--;
			playerHitIndex = -1;
		}

#pragma endregion
#pragma region Player
		glUseProgram(player.model.shaderProgram);
		//lighting uniforms
		//get and set light colour and position uniform
		lightColLocation = glGetUniformLocation(player.model.shaderProgram, "lightCol1");
		glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour1));
		lightPositionLocation = glGetUniformLocation(player.model.shaderProgram, "lightPos1");
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition1));
		//get and set the ligh color and position for light two
		lightColLocation = glGetUniformLocation(player.model.shaderProgram, "lightCol2");
		glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour2));
		lightPositionLocation = glGetUniformLocation(player.model.shaderProgram, "lightPos2");
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition2));

		//rotation
		//modelRotate = glm::rotate(modelRotate, (float)elapsedTime / 2000, glm::vec3(0.0f, 1.0f, 0.0f));
		importModelLocation = glGetUniformLocation(player.model.shaderProgram, "uModel");
		glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(translate *rotate  * bubbleScale));
		importViewLocation = glGetUniformLocation(player.model.shaderProgram, "uView");
		glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		viewPos = glGetUniformLocation(player.model.shaderProgram, "viewPos");
		glUniform3fv(viewPos, 1, glm::value_ptr(cam.cameraPosition));
		importProjectionLocation = glGetUniformLocation(player.model.shaderProgram, "uProjection");
		glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		//So we calculate the normal matrix in local space
		normalMatrix = glm::transpose(glm::inverse(translate *rotate*bubbleScale * viewMatrix));
		//set the normalMatrix in the shaders
		glUseProgram(player.model.shaderProgram);
		normalMatrixLocation = glGetUniformLocation(player.model.shaderProgram, "uNormalMatrix");
		glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, player.tex.texture);
		player.model.render();
#pragma endregion

#pragma region Bullet
		glUseProgram(bullet.model.shaderProgram);
		//lighting uniforms
		//get and set light colour and position uniform for light one
		lightColLocation = glGetUniformLocation(bullet.model.shaderProgram, "lightCol1");
		glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour1));
		lightPositionLocation = glGetUniformLocation(bullet.model.shaderProgram, "lightPos1");
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition1));
		//rotation
		//modelRotate = glm::rotate(modelRotate, (float)elapsedTime / 2000, glm::vec3(0.0f, 1.0f, 0.0f));
		importModelLocation = glGetUniformLocation(bullet.model.shaderProgram, "uModel");
		glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(translateBullet *rotateBullet  * bubbleScale));
		importViewLocation = glGetUniformLocation(bullet.model.shaderProgram, "uView");
		glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		viewPos = glGetUniformLocation(bullet.model.shaderProgram, "viewPos");
		glUniform3fv(viewPos, 1, glm::value_ptr(cam.cameraPosition));
		importProjectionLocation = glGetUniformLocation(bullet.model.shaderProgram, "uProjection");
		glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		//So we calculate the normal matrix in local space
		normalMatrix = glm::transpose(glm::inverse(translateBullet*rotateBullet* bubbleScale * viewMatrix));
		//set the normalMatrix in the shaders
		glUseProgram(bullet.model.shaderProgram);
		normalMatrixLocation = glGetUniformLocation(bullet.model.shaderProgram, "uNormalMatrix");
		glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, bullet.tex.texture);
		bullet.model.render();
#pragma endregion

#pragma region Boundry
		glUseProgram(boundry.model.shaderProgram);
		//get and set the light color and possition for dynamic light on the player
		lightColLocation = glGetUniformLocation(boundry.model.shaderProgram, "lightCol3");
		glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour3));
		lightPositionLocation = glGetUniformLocation(boundry.model.shaderProgram, "lightPos3");
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition3));

		//rotation
		//modelRotate = glm::rotate(modelRotate, (float)elapsedTime / 2000, glm::vec3(0.0f, 1.0f, 0.0f));
		importModelLocation = glGetUniformLocation(boundry.model.shaderProgram, "uModel");
		glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(boundryTranslate));
		importViewLocation = glGetUniformLocation(boundry.model.shaderProgram, "uView");
		glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		viewPos = glGetUniformLocation(boundry.model.shaderProgram, "viewPos");
		glUniform3fv(viewPos, 1, glm::value_ptr(cam.cameraPosition));
		importProjectionLocation = glGetUniformLocation(boundry.model.shaderProgram, "uProjection");
		glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		//So we calculate the normal matrix in local spac
		normalMatrix = glm::transpose(glm::inverse(boundryTranslate * viewMatrix));
		//set the normalMatrix in the shaders
		glUseProgram(boundry.model.shaderProgram);
		normalMatrixLocation = glGetUniformLocation(boundry.model.shaderProgram, "uNormalMatrix");
		glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glBindTexture(GL_TEXTURE_2D, boundry.tex.texture);
		boundry.model.render();
#pragma endregion

#pragma region SkyBox;
		glDepthFunc(GL_LEQUAL);
		glUseProgram(skybox.shaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(skybox.shaderProgram, "uView"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(cam.GetViewMatrix()))));
		glUniformMatrix4fv(glGetUniformLocation(skybox.shaderProgram, "uProjection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glBindVertexArray(skybox.VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture.texture);
		skybox.render();
		glDepthFunc(GL_LESS);

#pragma endregion
#pragma region Lives
		LivesPos = glm::mat4(1.0f); //resets lives possition
		LivesPos = glm::translate(LivesPos, glm::vec3(-1.8f, -1.4f, -3.5f) + cam.cameraPosition); //sets the possition equal to the bottom left of the camera.
		for (int i = 0; i < playerLives; i++) {
			glUseProgram(lives[i].shaderProgram);
			importModelLocation = glGetUniformLocation(lives[i].shaderProgram, "uModel");
			glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(LivesPos * UIScale));
			importViewLocation = glGetUniformLocation(lives[i].shaderProgram, "uView");
			glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			importProjectionLocation = glGetUniformLocation(lives[i].shaderProgram, "uProjection");
			glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			glBindTexture(GL_TEXTURE_2D, lives[i].tex.texture);
			lives[i].render();
			LivesPos = glm::translate(LivesPos, glm::vec3(0.3f, 0.0f, 0.0f));
		}
#pragma endregion

		SDL_GL_SwapWindow(sdl.win);

		//spawns new bubbes
		if (rand() % 100 == 5) {
			circlesActive2++;
			bubbles.push_back(Bubble());
			bubblesTranslate.push_back(glm::mat4(1.0f));

			bubbles[circlesActive2 - 1].setBuffers();
			randValue = (float)rand() / RAND_MAX * (rand() % 2 == 1) ? -2.0f : 2.0f;
			randValue2 = (float)rand() / RAND_MAX * (rand() % 2 == 1) ? -2.0f : 2.0f;;
			bubbles[circlesActive2 - 1].x = centreX + randValue;
			bubbles[circlesActive2 - 1].y = centreY + randValue2;
			bubblesTranslate[circlesActive2 - 1] = glm::translate(bubblesTranslate[circlesActive2 - 1], glm::vec3(bubbles[circlesActive2 - 1].x, bubbles[circlesActive2 - 1].y, 0.0f));
			std::cout << "Bubble created at " << bubbles[circlesActive2 - 1].x << " " << bubbles[circlesActive2 - 1].y << std::endl;
		}

		if (playerLives == 0) {
			windowOpen = false;
		}

	}//end loop

	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;
}
bool collisions(Bullet bullet, Bubble c) {

	float Mag = (bullet.x - c.x) * (bullet.x - c.x) + (bullet.y - c.y) * (bullet.y - c.y);
	Mag += sqrt(Mag);
	std::cout << Mag << std::endl;
	if (Mag < .2f) {
		//find if circle is touching
		std::cout << "Colliding" << std::endl;
		return true;
	}
	return false;
}

bool collisions(Player player, Bubble &c) {
	float Mag = (player.x - c.x) * (player.x - c.x) + (player.y - c.y) * (player.y - c.y);
	Mag += sqrt(Mag);
	std::cout << Mag << std::endl;
	if (Mag < .25f) {
		//find if circle is touching
		std::cout << "Colliding" << std::endl;
		playerLives--;
		return true;
	}
	return false;
}

Bubble updatePositions(Bubble c, int i)
{
	//update positions of the bubbles

	if ((c.x > rightScreen) && (c.xSpeed > 0)) {
		c.xSpeed *= -1.0f;
		bubblesTranslate[i] = glm::translate(bubblesTranslate[i], glm::vec3(c.xSpeed, c.ySpeed, 0.0f));
		c.x += c.xSpeed;
		c.y += c.ySpeed;

		return c;
	}
	if ((c.xSpeed < 0) && (c.x < leftScren)) {
		c.xSpeed *= -1.0f;
		bubblesTranslate[i] = glm::translate(bubblesTranslate[i], glm::vec3(c.xSpeed, c.ySpeed, 0.0f));
		c.x += c.xSpeed;
		c.y += c.ySpeed;

		return c;
	}
	if ((c.y > topScreen) && (c.ySpeed > 0)) {
		c.ySpeed *= -1.0f;
		bubblesTranslate[i] = glm::translate(bubblesTranslate[i], glm::vec3(c.xSpeed, c.ySpeed, 0.0f));
		c.x += c.xSpeed;
		c.y += c.ySpeed;

		return c;
	}
	if ((c.ySpeed < 0) && (c.y < bottonScreen)) {
		c.ySpeed *= -1.0f;
		bubblesTranslate[i] = glm::translate(bubblesTranslate[i], glm::vec3(c.xSpeed, c.ySpeed, 0.0f));
		c.x += c.xSpeed;
		c.y += c.ySpeed;

		return c;
	}
	if (true)
	{
		bubblesTranslate[i] = glm::translate(bubblesTranslate[i], glm::vec3(c.xSpeed, c.ySpeed, 0.0f));
		c.x += c.xSpeed;
		c.y += c.ySpeed;
		return c;
	}

}

void handleInput(Player &player, Bullet &bullet)
{


	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			windowOpen = false;
		}
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				std::cout << "Window resized w:" << w << " h:" << h << std::endl;
				SDL_GetWindowSize(win, &w, &h);
				newwidth = h * aspect;
				left = (w - newwidth) / 2;
				glViewport(left, 0, newwidth, h);
				break;


			default:
				break;
			}
		}
		//looks at any input
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_q) {
				//ADD IN FULL SCREEN CODE HERE;
			}

			if (event.key.keysym.sym == SDLK_w) {
				//translate = glm::translate(translate, glm::vec3((float)cos(angle)*0.02f, (float)sin(angle)*0.02f, 0.0f));
				keypressed[0] = true;
			}
			if (event.key.keysym.sym == SDLK_a) {
				keypressed[1] = true;
			}
			if (event.key.keysym.sym == SDLK_d) {
				keypressed[2] = true;
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				//spawn bullet
				keypressed[3] = true;
			}
			if (event.key.keysym.sym == SDLK_TAB) {
				isFullScreen = !isFullScreen;
				if (isFullScreen)
					SDL_SetWindowFullscreen(win, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
				else
					SDL_SetWindowFullscreen(win, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

			}

			if (event.key.keysym.sym == SDLK_UP) {
				cam.camZPos += 0.5f;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				cam.camZPos += -0.5f;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				cam.camXPos += (float)cos(2)*0.2f;
				cam.camYPos += (float)sin(2)*0.2f;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				cam.camXPos -= (float)cos(2)*0.2f;
				cam.camYPos -= (float)sin(2)*0.2f;
			}


		}

		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_w) {
				keypressed[0] = false;
			}
			if (event.key.keysym.sym == SDLK_a) {
				keypressed[1] = false;
			}
			if (event.key.keysym.sym == SDLK_d) {
				keypressed[2] = false;
			}
			if (event.key.keysym.sym == SDLK_SPACE) {
				keypressed[3] = false;
			}
		}

	}

	//goes through the input
	if (keypressed[0]) {
		if (player.x + (float)cos(angle)*0.02f < rightScreen && player.x + (float)cos(angle)*0.02f > leftScren && player.y + (float)sin(angle)*0.02f > bottonScreen && player.y + (float)sin(angle)*0.02f < topScreen) {
			translate = glm::translate(translate, glm::vec3((float)cos(angle)*0.02f, (float)sin(angle)*0.02f, 0.0f));
			player.x += (float)cos(angle)*0.02f;
			player.y += (float)sin(angle)*0.02f;
		}
		else {
			angle += glm::radians(180.0f);
			rotate = glm::rotate(rotate, glm::radians(180.0f), glm::vec3(0, 1, 0));
		}
	}
	if (keypressed[1]) {
		angle += glm::radians(10.0f);
		rotate = glm::rotate(rotate, glm::radians(10.0f), glm::vec3(0, 1, 0));
	}
	if (keypressed[2]) {
		angle -= glm::radians(10.0f);
		rotate = glm::rotate(rotate, glm::radians(-10.0f), glm::vec3(0, 1, 0));
	}
	if (keypressed[3]) {
		keypressed[3] = false;
		bullet.x = player.x;
		bullet.y = player.y;
		translateBullet = translate;
		rotateBullet = rotate;
		bulletAngle = angle;
		index++;
		translateBullet = glm::translate(translateBullet, glm::vec3((float)cos(angle)*0.03f, (float)sin(angle)*0.03f, 0.0f));
	}
	translateBullet = glm::translate(translateBullet, glm::vec3((float)cos(bulletAngle)*0.03f, (float)sin(bulletAngle)*0.03f, 0.0f));
	bullet.x += (float)cos(bulletAngle)*0.03f;
	bullet.y += (float)sin(bulletAngle)*0.03f;


}
#endif
#endif
#endif
#endif