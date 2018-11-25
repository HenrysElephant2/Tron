#include "Gameplay.h"

GameState::GameState() {
	previousTime = 0;
}

// Should return elapsed time in seconds
double GameState::getElapsedTime() {
	Uint64 currentTime = SDL_GetPerformanceCounter();
	double deltaTime = (double)((currentTime - previousTime) * 1000 / (double)SDL_GetPerformanceFrequency());
	previousTime = currentTime;
	return deltaTime / 1000;
}