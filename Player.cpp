class Player {
private:
	double x,y,z; // location of the player
	double forward_x, forward_y, forward_z; // vector that is the direction that the bike is pointed
	double up_x, up_y, up_z; // vector pointing up from the player to determine tilt of the player's model
	double velocity; // velocity of the player as a number
	bool trail_on;
	Trail trail;
	Hitbox hitbox;

public:

	// constructor for the player
	Player(double loc_x, double loc_y, double loc_z, double dir_x, double dir_y, double dir_z){
		x = loc_x;
		y = loc_y;
		z = loc_z;
		// turn direction vector into unit vector
		double m = sqrt(dir_x*dir_x + dir_y*dir_y + dir_z*dir_z);
		forward_x = dir_x/m;
		forward_y = dir_y/m;
		forward_z = dir_z/m;

		trail_on = false;
	}

	// turns on the trail of the bike. Once turned on, cannot be turned off
	void beginTrail()
	{
		trail_on = true;
	}

	// move the player in the current direction it is pointed based on its velocity
	void movePlayer()
	{
		//TODO
	}

	// do all the opengl to render the model for the player model. will call the trail render through this
	void display()
	{
		// TODO
	}

	Trail getTrail()
	{
		return trail;
	}

	void getLocation(double * ret_x, double * ret_y, double * ret_z)
	{
		*ret_x = x;
		*ret_y = y;
		*ret_z = z;
	}

	Hitbox getHitbox()
	{
		return hitbox;
	}
};


int main() {
	return 0;
}