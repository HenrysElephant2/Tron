#include "Player.h"


// default constructor
Player::Player(){}

// constructor for the player
Player::Player(double loc_x, double loc_y, double loc_z, double dir_x, double dir_y, double dir_z, Model * m, Vector c ){
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

	alive = true;
	left = false;
	right = false;

	velocity = MOVE_RATE;
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
	unsigned int trailTexture = LoadTexBMP("trail.bmp");
	trail_on = true;
	trail = new Trail( getTrailBottom(), getTrailTop(), limit, trailTexture, color );
}

// move the player in the current direction it is pointed based on its velocity
void Player::movePlayer( double dt )
{
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
	trail->update( getTrailBottom(), getTrailTop() );
}

void Player::commitNotAlive() {
	alive = false;
}

// do all the opengl to render the model for the player model. will call the trail render through this
void Player::display( TransparentRenderer *tr, Vector *cameraPos )
{
	// hitbox->renderSelf(false);
	glColor3d(color.getX(), color.getY(), color.getZ());
	if(model != NULL)
		model->display(loc,direction,tilt_vector,PLAYER_SCALE);

	if( trail != NULL ) {
		Vector emmiter = Add0( *loc, Scale0(*direction, -10) );
		trail->stage( emmiter, tilt_vector, tr, cameraPos );
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

