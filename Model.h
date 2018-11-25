
#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <sstream>
#include <cstdio>
#include "Vector.h"
#include "LoadModel.h"

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

public:

	Model(char * filename);
	Model(char * filename, char * texturename);
	~Model();

	void display(Vector * v, Vector * direction, Vector * up, double s);
};

#endif