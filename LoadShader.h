
#ifndef LOAD_SHADER_H
#define LOAD_SHADER_H

#define GL_GLEXT_PROTOTYPES
#include "LoadModel.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
// #include <GLUT/glut.h>
// #include <OpenGL/gl.h>
// #include <OpenGL/glext.h>
//#include <OpenGL/gl.h>



#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#ifdef __cplusplus
extern "C" {
#endif


int CreateShader(GLenum type,char* file);
char* ReadText(char *file);
void PrintShaderLog(int obj,char* file);
void PrintProgramLog(int obj);
int CreateShaderProg(char* VertFile,char* FragFile);

#ifdef __cplusplus
}
#endif
#endif