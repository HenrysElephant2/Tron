#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __cplusplus
extern "C" {
#endif



void LoadModel(char * filename, float ** vertices_return, int * num_v_return, 
								float ** uv_return, int * num_uv_return,
								float ** normals_return, int * num_norm_return,
								int ** faces_return, int * num_f_return);

void readcoord(char * line, int num_coord, float ** array, int num_in_array, int size);
void readIntCoord(char * line, int num_coord, int ** array, int num_in_array, int size);
void readfloat(char * line, int n, float * loc);
void readInts(char * line, int n, int * loc);
void normalizeDelimiter(char * line);


//texture loader taken from in class examples
// #define Cos(th) cos(3.1415926/180*(th))
// #define Sin(th) sin(3.1415926/180*(th))


void ErrCheck(const char* where);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);


#ifdef __cplusplus
}
#endif
#endif