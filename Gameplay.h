// Two player first-person game

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iostream>
#include "LoadShader.h"
#include "GameState.h"
#include "Player.h"
#include "Map.h"
#include "Tile.h"
#include "Vector.h"
#include "Model.h"
#include "TransparentRenderer.h"


#ifndef MATH_FUNCTIONS
#define MATH_FUNCTIONS
#define Cos(x) cos(3.1415926535/180*(x))
#define Sin(x) sin(3.1415926535/180*(x))
#endif

#define STATE_WAITING 0
#define STATE_PLAYING 1
#define STATE_P1_WIN 2
#define STATE_P2_WIN 3
#define STATE_TIE 4

#define NUMBER_GAUSSIAN_PASSES 8

class Gameplay: public GameState {
private:
	double ph;      //  Elevation of view angle
	double th;      //  Azimuth of view angle
	bool thup, thdown, phup, phdown; // Booleans to control window movement

	Player *player1, *player2;
	Vector p1color, p2color;
	Map *map;
	int state;
	Model * bike;

	// shader programs
	GLuint bikeProgram;
	GLuint blurProgram;
	GLuint blendProgram;
	GLuint brightProgram;
	GLuint bikeBrightProgram;

	// frame buffers and textures for scene
	GLuint frameBuffer; // frame buffer that scene is rendered to
	GLuint renderNormalTexture; // texture for frameBuffer to render to

	// frame buffer and texture for bright parts of scene
	GLuint blurBuffer; // frame buffer that bright spots in scene are rendered to
	GLuint blurTexture; // texture for blurBuffer to render to

	// frame buffer and textures for guassian blur
	GLuint pingpongFrameBuffer[2];
	GLuint pingpongTextures[2];

	// color attachments
	GLenum colorAttachments[1] = {GL_COLOR_ATTACHMENT0};

	// locations of variables in shader programs
	GLuint horizontalLocation;
	GLuint bloomBoolLoc;
	GLuint exposureLoc;
	GLuint widthLoc;
	GLuint heightLoc;

	// blend program variables - exposure currently turned off in shader
	float exposure = .1f;
	bool bloomOn = true;

	static const int SCREEN_WIDTH = 1400;
	static const int SCREEN_HEIGHT = 900;


public:
	Gameplay();
	~Gameplay();

	// Inherited methods
	void keyDown(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT
	void keyUp(SDL_Keycode key, int x, int y); // need to format this specifically for SDL instead of GLUT

	void special(int key, int x, int y); /*NEEDED in GLUT, maybe different in other system */
	void mouse(/*TODO*/);
	void display();
	void update();

	void reset();
	void displayAll( Vector *cameraPos, bool renderBloom);

	void render2DScreen();
	void postProcessingSetup();
	void postProcessingStep2();
	void postProcessing();
};

#endif