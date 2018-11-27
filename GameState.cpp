#include "Gameplay.h"

GameState::GameState() {
	wWidth = 0;
	wHeight = 0;
	previousTime = 0;
	fov = 45;
	asp = 1.4;
	split = false;
}

// Should return elapsed time in seconds
double GameState::getElapsedTime() {
	Uint64 currentTime = SDL_GetPerformanceCounter();
	double deltaTime = (double)((currentTime - previousTime) * 1000 / (double)SDL_GetPerformanceFrequency());
	previousTime = currentTime;
	return deltaTime / 1000;
}


// Modified Project function from ex9
void GameState::Project( bool split )
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    split ? gluPerspective(fov*2,asp*2,1,2000) : gluPerspective(fov,asp,10,5000);
    // gluPerspective(fov,asp,10,5000);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
}

// reshape function from ex9
void GameState::reshape(int width,int height) {
	wWidth = width;
	wHeight = height;
	if( split ) {
		reshapeSplit(width, height);
		return;
	}
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project(false);
}

// reshape function from ex9 - need to call viewport in screen
void GameState::reshapeSplit(int width,int height) {
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set projection
    Project(true);
}