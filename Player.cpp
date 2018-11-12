#include "Player.h"

#define MOVE_RATE 100 // divides the movement by move_rate value, essentially move_rate 
						// is how many ticks for the model to move forward 1 unit
#define TILT_RATE .01 // how quickly the model tilts while a turn key is pressed. radians per tick
#define MAX_TILT .52 // max radians that the model can tilt while turning
#define TURN_RATE .01 // rate of which the bike turns per tick. radians per tick

	// constructor for the player
	Player::Player(double loc_x, double loc_y, double loc_z, double dir_x, double dir_y, double dir_z){
		loc = new Vector(loc_x, loc_y, loc_z);
		// turn direction vector into unit vector

		double m = sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z);
		direction = new Vector(dir_x/m, dir_y/m, dir_z/m);

		up_vector = new Vector(0,1,0);
		tilt_vector = new Vector(0,1,0);
		trail_on = false;
		angle = 0;
		turn = 0;
	}

	// turns on the trail of the bike. Once turned on, cannot be turned off
	void Player::beginTrail()
	{
		trail_on = true;
	}

	// move the player in the current direction it is pointed based on its velocity
	void Player::movePlayer()
	{
		//TODO
		// move model forward
		loc->Add(direction,1/MOVE_RATE);

		//turn forward vector
		if(turn != 0)
			direction->Rotate(turn*TURN_RATE, up_vector);


		// change tilt of model
		double old_angle = angle;
		angle += turn*TILT_RATE; // tilt if key is pressed
		// make sure angle of tilt doesn't go over or under max
		if(angle < -MAX_TILT) 
			angle = -MAX_TILT;
		if(angle > MAX_TILT)
			angle = MAX_TILT;
		// tilt back to angle 0 if no key is pressed
		if(turn == 0 && angle != 0)
		{
			if(angle < 0)
				angle += TILT_RATE;
			else angle -= TILT_RATE;
		}
		if(angle < .05 && angle > -.05)
		{
			angle = 0;
		}
		if(angle != 0)
			tilt_vector->Rotate(angle - old_angle, up_vector);
		else if (old_angle != angle){
			tilt_vector->setX(0);
			tilt_vector->setY(1);
			tilt_vector->setZ(0);
		}
	}

	// do all the opengl to render the model for the player model. will call the trail render through this
	void Player::display()
	{
		// TODO
	}

	Trail Player::getTrail()
	{
		return trail;
	}

	Vector * Player::getLocation()
	{
		return loc;
	}

	Hitbox * Player::getHitbox()
	{
		return hitbox;
	}

	void Player::keyPressed(SDL_Event event)
	{
		switch(event.key.keysym.sym){
			case SDLK_d:
				turn += 1;
				break;
			case SDLK_a:
				turn -= 1;
				break;
			default:
				break;
			}
	}

	void Player::keyReleased(SDL_Event event)
	{
		switch(event.key.keysym.sym){
			case SDLK_d:
				turn -= 1;
				break;
			case SDLK_a:
				turn += 1;
				break;
			default:
				break;
			}
	}

	
int main(int argc, char * args[])
{return 0;}
