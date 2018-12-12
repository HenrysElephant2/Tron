#include "Gameplay.h"

Gameplay::Gameplay(int w, int h) : GameState(w,h) {
	p1color = Vector(1,.3,0);
	p2color = Vector(.5,.8,1);
	init();
}

Gameplay::Gameplay( int w, int h, Vector p1c, Vector p2c ) : GameState(w,h) {
	p1color = p1c;
	p2color = p2c;
	init();
}

Gameplay::~Gameplay() {
	delete player1;
	delete player2;
	delete map;
}

void Gameplay::init() {
	player1 = new Player( 0, 0, 0, 0, 0, 1, bike, p1exp, p1color);
	player2 = new Player( (MAP_LENGTH-1)*TILE_SIZE, 0, (MAP_WIDTH-1)*TILE_SIZE, 0, 0, -1, bike, p2exp, p2color);

	unsigned int floorTex = LoadTexBMP("Textures/tile.bmp");
	unsigned int wallTex = LoadTexBMP("Textures/wall.bmp");
	unsigned int skyTex = LoadTexBMP("Textures/sky.bmp");
	map = new Map( MAP_LENGTH, 1, MAP_WIDTH, floorTex, wallTex, skyTex );

	state = STATE_WAITING;

	timer = 0;

	ph = 15;
	th = 45;

	split = true; // Split screen

	resetButton = Button(.5,.5,.5,.125,buttonTexture2,0,1,.75,1,menuButtonColor);
	menuButton = Button(.5,.35,.5,.125,buttonTexture2,0,1,.502,.75,menuButtonColor);
	p1winsLogo = Button(.5,.65,.8,.2,labelTextures,0,1,.5,.75,p1color);
	p2winsLogo = Button(.5,.65,.8,.2,labelTextures,0,1,.25,.5,p2color);
	tieLogo = Button(.5,.65,.8,.2,labelTextures,0,1,0,.25,menuButtonColor);
	spaceStart = Button(.5,.5,1,.25,labelTextures2,0,1,.75,1,menuButtonColor);
	p1Controls = Button(.5,.7,.5,.125,labelTextures2,0,1,.5,.75,p1color);
	p2Controls = Button(.5,.2,.5,.125,labelTextures2,0,1,.25,.5,p2color);
}

void Gameplay::keyDown( SDL_Keycode key ) {
	switch( key ) {
		case SDLK_a: player1->setLeft(true); break;
		case SDLK_d: player1->setRight(true); break;

		case SDLK_j: player2->setLeft(true); break;
		case SDLK_l: player2->setRight(true); break;

		case SDLK_SPACE:
			if( state == STATE_WAITING ) {
				state = STATE_PLAYING;
				player1->beginTrail(true);
				player2->beginTrail(true);
			}
			else if( state == STATE_PAUSED )
				state = STATE_PLAYING;
			else
				state = STATE_PAUSED;
		break;

		case SDLK_r:
			if( state == STATE_P1_WIN || state == STATE_P2_WIN || state == STATE_TIE )
				reset();
		break;

		case SDLK_1: bloomOn = true; break;
		case SDLK_2: bloomOn = false; break;
	}
}

void Gameplay::keyUp( SDL_Keycode key ) {
	switch( key ) {
		case SDLK_a: player1->setLeft(false); break;
		case SDLK_d: player1->setRight(false); break;

		case SDLK_j: player2->setLeft(false); break;
		case SDLK_l: player2->setRight(false); break;
	}
}

void Gameplay::mouseDown(int x, int y) {
	if( state == STATE_TIE || state == STATE_P1_WIN || state == STATE_P2_WIN ) {
		if( resetButton.testClicked(x,y,wWidth,wHeight) )
			mouseState = RESET_DOWN;
		else if( menuButton.testClicked(x,y,wWidth,wHeight) )
			mouseState = MENU_DOWN;
	}
}

void Gameplay::mouseUp(int x, int y) {
	if( state == STATE_TIE || state == STATE_P1_WIN || state == STATE_P2_WIN ) {
		if( mouseState == RESET_DOWN && resetButton.testClicked(x,y,wWidth,wHeight) )
			reset();
		else if( mouseState == MENU_DOWN && menuButton.testClicked(x,y,wWidth,wHeight) ) {
			GameState *newState = new Menu(wWidth, wHeight);
			setNextState( newState );
		}
	}
}

void Gameplay::update() {
	double tDiff = getElapsedTime();

	if( state != STATE_WAITING && state != STATE_PAUSED && timer < 5 ) {
		player1->movePlayer(tDiff);
		player2->movePlayer(tDiff);

		if( testCollision( player1->getHitbox(), player2->getHitbox() ) ) {
			if( state == STATE_PLAYING ) state = STATE_PENDING;
			player1->commitNotAlive();
			player2->commitNotAlive();
		}
		if( player1->getTrail()->testTrailHit( player2->getHitbox() ) || player2->getTrail()->testTrailHit( player2->getHitbox() ) || map->testWallHits( player2->getHitbox() ) ) {
			if( state == STATE_PLAYING ) state = STATE_PENDING;
			player2->commitNotAlive();
		}
		if( player2->getTrail()->testTrailHit( player1->getHitbox() ) || player1->getTrail()->testTrailHit( player1->getHitbox() ) || map->testWallHits( player1->getHitbox() ) ) {
			if( state == STATE_PLAYING ) state = STATE_PENDING;
			player1->commitNotAlive();
		}

		if( state != STATE_PLAYING )
			timer += tDiff;
		if( state == STATE_PENDING && timer > 2.0 ) {
			if( player1->isAlive() )
				state = STATE_P1_WIN;
			else if( player2->isAlive() )
				state = STATE_P2_WIN;	
			else
				state = STATE_TIE;
		}
	}

	if( split && timer > 5 ) {
		split = false;
		reshape( wWidth, wHeight );
	}

	th += 20 * tDiff;
}

void Gameplay::reset() {
	delete player1;
	delete player2;

	timer = 0;
	
	p1exp->reset();
	p2exp->reset();

	player1 = new Player( 0, 0, 0, 0, 0, 1, bike, p1exp, p1color );
	player2 = new Player( (MAP_LENGTH-1)*TILE_SIZE, 0, (MAP_WIDTH-1)*TILE_SIZE, 0, 0, -1, bike, p2exp, p2color);

	state = STATE_WAITING;

	split = true;
	reshape( wWidth, wHeight );
}

void Gameplay::display() {

// 	if(!player1->isAlive())
// 		p1exp->calculate();
// 	if(!player2->isAlive())
// 		p2exp->calculate();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();

	if( split ) {
		Vector topView = player1->getViewLocation();
		Vector topTarget = player1->getViewTarget();
		Vector bottomView = player2->getViewLocation();
		Vector bottomTarget = player2->getViewTarget();

		//render to renderNormalTexture - renders the scene based on textures and colors
		postProcessingSetup();
		// Top window
		glViewport(0, wHeight/2, wWidth, wHeight/2);
		gluLookAt(topView.getX(), topView.getY(), topView.getZ(),
			      topTarget.getX(), topTarget.getY(), topTarget.getZ(),
			      0,1,0);
		displayAll( &topView, false);

		// Bottom window
		glViewport(0, 0, wWidth, wHeight/2);
		glLoadIdentity();
		gluLookAt(bottomView.getX(), bottomView.getY(), bottomView.getZ(),
			      bottomTarget.getX(), bottomTarget.getY(), bottomTarget.getZ(),
				  0,1,0);
		displayAll( &bottomView, false);
	}
	else {
		double Ex = -350*Sin(th)*Cos(ph) + (MAP_LENGTH-1)/2.0*TILE_SIZE;
		double Ey = +350*Sin(ph);
		double Ez = +350*Cos(th)*Cos(ph) + (MAP_LENGTH-1)/2.0*TILE_SIZE;
		Vector viewLoc(Ex, Ey, Ez);
		gluLookAt(Ex,Ey,Ez , (MAP_LENGTH-1)/2.0*TILE_SIZE,0,(MAP_LENGTH-1)/2.0*TILE_SIZE , 0,Cos(ph),0);
		postProcessingSetup();
		displayAll( &viewLoc,false);
		postProcessingStep2();
		displayAll( &viewLoc,true);
	}
	

	// Any overlays
	glViewport(0,0,wWidth,wHeight);
	switchToOverlay();

	if( state == STATE_WAITING ) {
		spaceStart.display();
		p1Controls.display();
		p2Controls.display();
	}
	else if( timer > 2 ) {
		if( state == STATE_TIE || state == STATE_P1_WIN || state == STATE_P2_WIN ) {
			resetButton.display();
			menuButton.display();
		}
		if( state == STATE_TIE )
			tieLogo.display();
		else if( state == STATE_P1_WIN )
			p1winsLogo.display();
		else if( state == STATE_P2_WIN )
			p2winsLogo.display();
	}

	// Switch back
	switchTo3D();

	postProcessing();
}

void Gameplay::displayAll( Vector *cameraPos, bool renderBloom ) {
	glUseProgram(renderBloom ? brightProgram:0);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

	map->displaySkybox();
	map->displayWalls();

	// Reflections/Underside of map
	Vector reflectCamera = Add0( *cameraPos, Vector(0,-2*cameraPos->getY(), 0) );
	TransparentRenderer *tr = new TransparentRenderer();
	glPushMatrix();
	glTranslated(0,-.12,0);
	glScaled(1,-1,1);
	glUseProgram(bikeProgram);
	player1->display(tr, &reflectCamera);
	glUseProgram(bikeProgram);
	player2->display(tr, &reflectCamera);
	glUseProgram(0);
	tr->display( &reflectCamera );
	glPopMatrix();
	delete tr;

	// Floor tiles
	map->displayTiles();

	// Top of map
	tr = new TransparentRenderer();
	glUseProgram(bikeProgram);
	player1->display(tr, cameraPos);
	glUseProgram(bikeProgram);
	player2->display(tr, cameraPos);
	glUseProgram(0);
	tr->display( cameraPos );
	delete tr;
}
