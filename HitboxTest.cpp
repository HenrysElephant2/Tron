#include "HitboxTest.h"

HitboxTest::HitboxTest() {
	Vector *p1 = new Vector(1,-1,0);
	Vector *f1 = new Vector(0,0,-1);
	Vector *u1 = new Vector(0,1,0);

	Vector *p2 = new Vector(-3,-5,0);
	Vector *f2 = new Vector(0,0,-1);
	Vector *u2 = new Vector(0,1,0);
	
	h1 = new Hitbox(p1, f1, u1, 2, 2, 2);
	h2 = new Hitbox(p2, f2, u2, 2, 2, 2);

	ph = 0;      //  Elevation of view angle
	th = 0;      //  Azimuth of view angle

	// Booleans to control window movement
	thup = false;
	thdown = false;
	phup = false;
	phdown = false;

	// Bools to move hitbox
	hup = false;
	hdown = false;
	hleft = false;
	hright = false;
	hforward = false;
	hbackward = false;

	// Bools to rotate hitbox
	pu = false;
	pd = false;
	yl = false;
	yr = false;
	rr = false;
	rl = false;

	colliding = testCollision( h1, h2 );
}

HitboxTest::~HitboxTest() {
    delete h1;
    delete h2;
}

void HitboxTest::keyDown( SDL_Keycode key, int x, int y ) {
    switch( key ) {
        case SDLK_w: phup = true; break;
        case SDLK_s: phdown = true; break;
        case SDLK_a: thdown = true; break;
        case SDLK_d: thup = true; break;
        case SDLK_o: hup = true; break;
        case SDLK_u: hdown = true; break;
        case SDLK_j: hleft = true; break;
        case SDLK_l: hright = true; break;
        case SDLK_i: hforward = true; break;
        case SDLK_k: hbackward = true; break;
        case SDLK_t: pu = true; break;
        case SDLK_g: pd = true; break;
        case SDLK_f: yl = true; break;
        case SDLK_h: yr = true; break;
        case SDLK_y: rr = true; break;
        case SDLK_r: rl = true; break;
    }
}

void HitboxTest::keyUp( SDL_Keycode key, int x, int y ) {
    switch( key ) {
        case SDLK_w: phup = false; break;
        case SDLK_s: phdown = false; break;
        case SDLK_a: thdown = false; break;
        case SDLK_d: thup = false; break;
        case SDLK_o: hup = false; break;
        case SDLK_u: hdown = false; break;
        case SDLK_j: hleft = false; break;
        case SDLK_l: hright = false; break;
        case SDLK_i: hforward = false; break;
        case SDLK_k: hbackward = false; break;
        case SDLK_t: pu = false; break;
        case SDLK_g: pd = false; break;
        case SDLK_f: yl = false; break;
        case SDLK_h: yr = false; break;
        case SDLK_y: rr = false; break;
        case SDLK_r: rl = false; break;
    }
}

void HitboxTest::special(int key, int x, int y) {} /*NEEDED in GLUT, maybe different in other system */
void HitboxTest::mouse(/*TODO*/) {}

void HitboxTest::update() {
    colliding = testCollision( h1, h2 );

    if( thup )
        th += 5;
    if( thdown )
        th -= 5;
    if( phup )
        ph += 5;
    if( phdown )
        ph -= 5;

    if( pu && !pd )
        h1->pitch(-rSpeed);
    if( pd && !pu )
        h1->pitch(rSpeed);
    if( yl && !yr )
        h1->yaw(rSpeed);
    if( yr && !yl )
        h1->yaw(-rSpeed);
    if( rl && !rr )
        h1->roll(-rSpeed);
    if( rr && !rl )
        h1->roll(rSpeed);


    Vector *moveVec = new Vector(0,0,0);
    if( hforward )
        moveVec->Add(0,0,-mSpeed);
    if( hbackward )
        moveVec->Add(0,0,mSpeed);
    if( hleft )
        moveVec->Add(-mSpeed,0,0);
    if( hright )
        moveVec->Add(mSpeed,0,0);
    if( hup )
        moveVec->Add(0,mSpeed,0);
    if( hdown )
        moveVec->Add(0,-mSpeed,0);
    h1->move( moveVec );
    delete moveVec;
}

void HitboxTest::display() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    // Set eye position
    double Ex = -.3*100*Sin(th)*Cos(ph);
    double Ey = +.3*100  *Sin(ph);
    double Ez = +.3*100*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    
    h1->renderSelf(colliding);
    h2->renderSelf(colliding);

    // glFlush();
    // glSwapBuffers();
}