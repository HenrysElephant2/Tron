
#ifndef EXPLOSION_H
#define EXPLOSION_H

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "LoadShader.h"
#include <iostream>

#define PARTICLE_TEXTURE_SIZE 512



class Explosion {
private:
	bool activeCalcTexture; // which texture to use for calculations
	bool init; // whether or not the particles have been initialized

	GLuint calcProgram;
	GLuint displayProgram;

	// frame buffer and textures for guassian blur
	GLuint pingpongFrameBuffer[2];
	GLuint pingpongTextures[2];

	// color attachments
	GLenum colorAttachments[1] = {GL_COLOR_ATTACHMENT0};

	GLuint initLocation;
	GLuint timeLocation;
	GLuint rainbowLocation;

	float previousTime;


public:
	Explosion();
	~Explosion();
	void calculate();
	void render2DScreen();
	void display();
	void reset();


};


#endif