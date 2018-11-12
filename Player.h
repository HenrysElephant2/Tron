#ifndef PLAYER_H
#define PLAYER_H

#include "Hitbox.h"
#include "Vector.h"
#include "Trail.h"
#include <SDL2/SDL.h>
#include <math.h>



class Player {
private:
	Vector * loc; // location of the player
	Vector * direction; // vector that is the direction that the bike is pointed
	Vector * up_vector; // vector pointing up from the player
	Vector * tilt_vector; // vector determining the tilt
	double angle; // angle of the player's tilt
	int turn; // int that represents how the player is turning. -1 for left, 1 for right, and 0 for going straight
	double velocity; // velocity of the player as a number
	bool trail_on;
	Trail trail;
	Hitbox * hitbox;

public:

	// constructor for the player
	Player(double loc_x, double loc_y, double loc_z, double dir_x, double dir_y, double dir_z);

	// turns on the trail of the bike. Once turned on, cannot be turned off
	void beginTrail();

	// move the player in the current direction it is pointed based on its velocity
	void movePlayer();

	// do all the opengl to render the model for the player model. will call the trail render through this
	void display();

	Trail getTrail();

	Vector * getLocation();

	Hitbox * getHitbox();

	void keyPressed(SDL_Event event);
	void keyReleased(SDL_Event event);
};
#endif