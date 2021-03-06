
#ifndef MODEL_H
#define MODEL_H

#define GL_GLEXT_PROTOTYPES
#include <string>
#include <sstream>
#include <cstdio>
#include "Vector.h"
#include "LoadModel.h"
#include <math.h>

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Model {
private:
	int num_vertices;
	int num_faces;
	int num_uv;
	int num_normal;
	float * vertices;
	float * uv;
	float * normals;
	int * faces;
	unsigned int texture;
	Model * child; // a way to set up models as a linked list. Use if there are multiple models that contribute to the same object

	double rainbowColors[7][3] = {
		{1,0,0},
		{1,.5,0},
		{1,1,0},
		{0,1,0},
		{0,1,1},
		{0,0,1},
		{1,0,1}
	};
	void setRainbowColor(float x, float time);

public:

	Model(char * filename);
	Model(char * filename, char * texturename);
	~Model();

	void append(Model * m); // add a model to the linked list
	void append(char * name, char * texname);
	void append(char * name);

	Model * getNext();

	void display(Vector * v, Vector * direction, Vector * up, double s, bool texOn);
	void displayBasic(bool texOn);

	void displayRainbow(Vector * v, Vector * direction, Vector * up, double s, bool texOn);
	void displayBasicRainbow(bool texOn);
	
};

#endif