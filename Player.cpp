#include "Player.h"


// default constructor
Player::Player(){}

// constructor for the player
Player::Player(double loc_x, double loc_y, double loc_z, double dir_x, double dir_y, double dir_z, Model * m, Explosion * explosion, Vector c){
	loc = new Vector(loc_x, loc_y, loc_z);

	// turn direction vector into unit vector
	direction = new Vector(dir_x, dir_y, dir_z);
	direction->Normalize();

	up_vector = new Vector(0,1,0);
	tilt_vector = new Vector(0,1,0);
	trail_on = false;
	trail = NULL;
	angle = 0;
	turn = 0;

	hitbox = new Hitbox( loc, direction, up_vector, PLAYER_WIDTH, PLAYER_LENGTH, PLAYER_HEIGHT );

	model = m;
	color = c;
	lastColor = c;

	alive = true;
	left = false;
	right = false;

	velocity = MOVE_RATE;
	exp = explosion;
	if( color.getX() == -1 )
		explosion->setRainbow(true);
}

// destructor for the player
Player::~Player() {
	delete loc;
	delete direction;
	delete up_vector;
	delete tilt_vector;
	delete hitbox;
	delete trail;
}

// turns on the trail of the bike. Once turned on, cannot be turned off
void Player::beginTrail( bool limit )
{
	unsigned int trailTexture = LoadTexBMP("Textures/trail.bmp");
	trail_on = true;
	trail = new Trail( getTrailBottom(), getTrailTop(), limit, trailTexture, color );
}

// move the player in the current direction it is pointed based on its velocity
void Player::movePlayer( double dt )
{	
	if(alive) {
		//turn forward vector
		turn = right - left;

		// change tilt of model
		double old_angle = angle;
		angle += turn*TILT_RATE*dt; // tilt if key is pressed
		// make sure angle of tilt doesn't go over or under max
		if(angle < -MAX_TILT) 
			angle = -MAX_TILT;
		if(angle > MAX_TILT)
			angle = MAX_TILT;
		// tilt back to angle 0 if no key is pressed
		if(turn == 0 && angle != 0)
		{
			double toChange = TILT_RATE*dt;
			if( fabs(angle) < toChange )
				angle = 0;
			else if(angle < 0)
				angle += toChange;
			else angle -= toChange;

		}
		if(angle != 0)
			tilt_vector->Rotate(angle - old_angle, direction);
		else if (old_angle != angle){
			tilt_vector->setX(0);
			tilt_vector->setY(1);
			tilt_vector->setZ(0);
		}
		if(angle != 0) {
			direction->Rotate(-angle*TURN_RATE*dt, up_vector);
			tilt_vector->Rotate(-angle*TURN_RATE*dt, up_vector);
		}

		loc->Add( direction, velocity*dt );
		hitbox->updateVecs( loc, direction, tilt_vector );
	}
	if(!this->isAlive())
		exp->calculate();

	trail->update( getTrailBottom(), getTrailTop(), (color.getX() == -1 ? getRainbowColor() : color) );
}

void Player::commitNotAlive() {
	if( alive ) {
		alive = false;
		trail->setAdd(false);
		lastColor = getRainbowColor();
	}
}

// do all the opengl to render the model for the player model. will call the trail render through this
void Player::display( TransparentRenderer *tr, Vector *cameraPos )
{
	if(model != NULL && alive) {
		if( color.getX() != -1 ) {
			glColor3d(color.getX(), color.getY(), color.getZ());
			model->display(loc,direction,tilt_vector,PLAYER_SCALE, alive);
		}
		else
			model->displayRainbow(loc,direction,tilt_vector,PLAYER_SCALE, alive);
	}


	if( trail != NULL ) {
		Vector emmiter = Add0( *loc, Scale0(*direction, -10) );
		trail->stage( emmiter, tilt_vector, tr, cameraPos, (alive?getRainbowColor():lastColor) );
	}

	if(!alive && exp != NULL)
	{
		double dx = direction->getX();
		double dy = direction->getY();
		double dz = direction->getZ();

		double ux = 0;
		double uy = 1;
		double uz = 0;

		//  Unit vector in direction of flght
		double D0 = sqrt(dx*dx+dy*dy+dz*dz);
		double X0 = dx/D0;
		double Y0 = dy/D0;
		double Z0 = dz/D0;
		//  Unit vector in "up" direction
		double D1 = sqrt(ux*ux+uy*uy+uz*uz);
		double X1 = ux/D1;
		double Y1 = uy/D1;
		double Z1 = uz/D1;
		//  Cross product gives the third vector
		double X2 = Y0*Z1-Y1*Z0;
		double Y2 = Z0*X1-Z1*X0;
		double Z2 = X0*Y1-X1*Y0;
		//  Rotation matrix
		double mat[16];
		mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
		mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
		mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
		mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

		//  Save current transforms
		glPushMatrix();
		//  Offset, scale and rotate
		glTranslated(loc->getX(),loc->getY(),loc->getZ());
		glMultMatrixd(mat);
		//glScaled(s,s,s);
		
		if(color.getX() != -1) glColor3d(color.getX(), color.getY(), color.getZ());
		exp->display();

		glPopMatrix();
	}
}

Trail* Player::getTrail()
{
	return trail;
}

Vector * Player::getLocation()
{
	return loc;
}

Vector* Player::getTrailBottom() {
	Vector *retVec = new Vector();
	*retVec = Add0( *loc, Scale0( *direction, -16 ) );
	return retVec;
}

Vector* Player::getTrailTop() {
	Vector *retVec = new Vector();
	*retVec = Add0( Add0( *loc, Scale0( *direction, -16 ) ), Scale0( *tilt_vector, TRAIL_HEIGHT ) );
	return retVec;
}

Hitbox * Player::getHitbox()
{
	return hitbox;
}

void Player::setLeft( bool newVal ) { left = newVal; }
void Player::setRight( bool newVal ) { right = newVal; }

void Player::setLoc( Vector newLoc ) { loc->set(&newLoc); }
void Player::setDir( Vector newDir ) { direction->set(&newDir); }
void Player::setColor( Vector newColor ) { color = newColor; }

Vector Player::getViewLocation() {
	return Add0( Add0( *loc, Scale0( *direction, -VIEW_DISTANCE ) ), Scale0( *up_vector, VIEW_HEIGHT ) );
}

Vector Player::getViewTarget() {
	return Add0( *loc, Scale0( *up_vector, TARGET_HEIGHT ) );
}

Vector Player::getRainbowColor() {
	double x = -4.08997; //-7.86188;
	float time = SDL_GetPerformanceCounter()/1000000000.0;
	double frequency = 2;
	double rawI = frequency * (time + x);
	int colorI = (int)rawI % 7;
	double weight = rawI - (int)rawI;
	double red   = rainbowColors[(colorI+1)%7][0] * weight + rainbowColors[colorI][0] * (1-weight);
	double green = rainbowColors[(colorI+1)%7][1] * weight + rainbowColors[colorI][1] * (1-weight);
	double blue  = rainbowColors[(colorI+1)%7][2] * weight + rainbowColors[colorI][2] * (1-weight);
	return Vector(red,green,blue);
}

bool Player::isAlive(){
	return alive;
}

