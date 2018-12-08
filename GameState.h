#ifndef GAMESTATE_H
#define GAMESTATE_H

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "LoadShader.h"
#include "TransparentRenderer.h"
#include "Model.h"
#include "Explosion.h"


class GameState {
private:
	Uint64 previousTime;
	GameState *nextState;

protected:
	int wWidth, wHeight, fov;
	double asp;
	bool split;

	Model * bike;

	static const int SCREEN_WIDTH = 1400;
	static const int SCREEN_HEIGHT = 900;

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
	//float bloomThreshold = .8;

	Explosion * exp;

public:
	GameState();

	virtual void keyDown(SDL_Keycode key, int x, int y) = 0; // need to format this specifically for SDL instead of GLUT
	virtual void keyUp(SDL_Keycode key, int x, int y) = 0; // need to format this specifically for SDL instead of GLUT

	virtual void mouseDown(int x, int y) = 0;
	virtual void mouseUp(int x, int y) = 0;

	virtual void special(int key, int x, int y) = 0; /*NEEDED in GLUT, maybe different in other system */
	virtual void display() = 0;
	virtual void update() = 0;

	double getElapsedTime();

	void Project( bool split );
	void reshape( int height, int width );
	void reshapeSplit( int height, int width );

	void render2DScreen();
	void postProcessingSetup();
	void postProcessingStep2();
	void postProcessing();

	void setNextState( GameState *newState );
	GameState* getNextState();
};

#endif