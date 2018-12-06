#include "Gameplay.h"

Gameplay::Gameplay() {
	p1color = Vector(1,.3,0);
	p2color = Vector(.5,.8,1);

	player1 = new Player( 0, 0, 0, 0, 0, 1, bike, p1color );
	player2 = new Player( (MAP_LENGTH-1)*TILE_SIZE, 0, (MAP_WIDTH-1)*TILE_SIZE, 0, 0, -1, bike, p2color );

	unsigned int floorTex = LoadTexBMP("tile.bmp");
	unsigned int wallTex = LoadTexBMP("wall.bmp");
	map = new Map( MAP_LENGTH, 1, MAP_WIDTH, floorTex, wallTex );

	state = STATE_WAITING;

	ph = 30;	  //  Elevation of view angle
	th = 45;	  //  Azimuth of view angle

	// Booleans to control window movement
	thup = false;
	thdown = false;
	phup = false;
	phdown = false;

	split = true; // Split screen
}

Gameplay::Gameplay( Vector p1c, Vector p2c ) {
	p1color = p1c;
	p2color = p2c;

	player1 = new Player( 0, 0, 0, 0, 0, 1, bike, p1color );
	player2 = new Player( (MAP_LENGTH-1)*TILE_SIZE, 0, (MAP_WIDTH-1)*TILE_SIZE, 0, 0, -1, bike, p2color );

	unsigned int floorTex = LoadTexBMP("tile.bmp");
	unsigned int wallTex = LoadTexBMP("wall.bmp");
	map = new Map( MAP_LENGTH, 1, MAP_WIDTH, floorTex, wallTex );

	state = STATE_WAITING;

	ph = 30;	  //  Elevation of view angle
	th = 45;	  //  Azimuth of view angle

	// Booleans to control window movement
	thup = false;
	thdown = false;
	phup = false;
	phdown = false;

	split = true; // Split screen
}

Gameplay::~Gameplay() {
	delete player1;
	delete player2;
	delete map;
}

void Gameplay::keyDown( SDL_Keycode key, int x, int y ) {
	switch( key ) {
		case SDLK_a: player1->setLeft(true); break;
		case SDLK_d: player1->setRight(true); break;

		case SDLK_j: player2->setLeft(true); break;
		case SDLK_l: player2->setRight(true); break;


		case SDLK_t: phup = true; break;
		case SDLK_g: phdown = true; break;
		case SDLK_f: thdown = true; break;
		case SDLK_h: thup = true; break;

		case SDLK_SPACE:
			if( state == STATE_WAITING ) {
				state = STATE_PLAYING;
				player1->beginTrail(true);
				player2->beginTrail(true);
			}
			else if( state == STATE_PLAYING ) {
				state = STATE_WAITING;
			}
		break;

		case SDLK_r:
			if( state == STATE_P1_WIN || state == STATE_P2_WIN || state == STATE_TIE ) {
				reset();
			}
		break;

		case SDLK_m:
			split = !split;
			reshape(wWidth, wHeight);
		break;

		case SDLK_1: bloomOn = true; break;
		case SDLK_2: bloomOn = false; break;
		//case SDLK_9: bloomThreshold -= .05; break;
		//case SDLK_0: bloomThreshold += .05; break;
	}
}

void Gameplay::keyUp( SDL_Keycode key, int x, int y ) {
	switch( key ) {
		case SDLK_a: player1->setLeft(false); break;
		case SDLK_d: player1->setRight(false); break;

		case SDLK_j: player2->setLeft(false); break;
		case SDLK_l: player2->setRight(false); break;


		case SDLK_t: phup = false; break;
		case SDLK_g: phdown = false; break;
		case SDLK_f: thdown = false; break;
		case SDLK_h: thup = false; break;
	}
}

void Gameplay::special(int key, int x, int y) {}
void Gameplay::mouseDown(int x, int y) {}
void Gameplay::mouseUp(int x, int y) {}

void Gameplay::update() {
	double tDiff = getElapsedTime();

	th += (thup-thdown)*50*tDiff;
	ph += (phup-phdown)*50*tDiff;

	if( state == STATE_PLAYING ) {
		player1->movePlayer(tDiff);
		player2->movePlayer(tDiff);

		if( testCollision( player1->getHitbox(), player2->getHitbox() ) ) {
			state = STATE_TIE;
			player1->commitNotAlive();
			player2->commitNotAlive();
		}
		if( player1->getTrail()->testTrailHit( player2->getHitbox() ) || player2->getTrail()->testTrailHit( player2->getHitbox() ) || map->testWallHits( player2->getHitbox() ) ) {
			state = STATE_P1_WIN;
			player2->commitNotAlive();
		}
		if( player2->getTrail()->testTrailHit( player1->getHitbox() ) || player1->getTrail()->testTrailHit( player1->getHitbox() ) || map->testWallHits( player1->getHitbox() ) ) {
			if( state == STATE_P1_WIN )
				state = STATE_TIE;
			else
				state = STATE_P2_WIN;
			player1->commitNotAlive();
		}
	}
}

void Gameplay::reset() {
	delete player1;
	delete player2;
	player1 = new Player( 0, 0, 0, 0, 0, 1, bike, p1color );
	player2 = new Player( (MAP_LENGTH-1)*TILE_SIZE, 1, (MAP_WIDTH-1)*TILE_SIZE, 0, 0, -1, bike, p2color );

	state = STATE_WAITING;
}

void Gameplay::display() {
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


		// render to blurTexture - render only the bright spots of the scene
		//postProcessingStep2();
		// Top window
		// glViewport(0, wHeight/2, wWidth, wHeight/2);
		// glLoadIdentity();
		// gluLookAt(topView.getX(), topView.getY(), topView.getZ(),
		// 	      topTarget.getX(), topTarget.getY(), topTarget.getZ(),
		// 	      0,1,0);
		// displayAll( &topView, true);

		// // Bottom window
		// glViewport(0, 0, wWidth, wHeight/2);
		// glLoadIdentity();
		// gluLookAt(bottomView.getX(), bottomView.getY(), bottomView.getZ(),
		// 	      bottomTarget.getX(), bottomTarget.getY(), bottomTarget.getZ(),
		// 		  0,1,0);
		// displayAll( &bottomView, true);

		postProcessing();


	}
	else {
		double Ex = -500*Sin(th)*Cos(ph) + 2*TILE_SIZE;
		double Ey = +500*Sin(ph);
		double Ez = +500*Cos(th)*Cos(ph) + 2*TILE_SIZE;
		Vector viewLoc(Ex, Ey, Ez);
		gluLookAt(Ex,Ey,Ez , 2*TILE_SIZE,0,2*TILE_SIZE , 0,Cos(ph),0);
		postProcessingSetup();
		displayAll( &viewLoc,false);
		postProcessingStep2();
		displayAll( &viewLoc,true);
		postProcessing();
	}

}

void Gameplay::displayAll( Vector *cameraPos, bool renderBloom) {

	glUseProgram(renderBloom ? brightProgram:0);
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

	map->displayWalls();

	// Reflections/Underside of map
	Vector reflectCamera = Add0( *cameraPos, Vector(0,-2*cameraPos->getY(), 0) );
	TransparentRenderer *tr = new TransparentRenderer();
	glPushMatrix();
	glTranslated(0,-.12,0);
	glScaled(1,-1,1);
	glUseProgram(renderBloom?bikeBrightProgram:bikeProgram);
	player1->display(tr, &reflectCamera);
	player2->display(tr, &reflectCamera);
	glUseProgram(renderBloom ? brightProgram:0);
	tr->display( &reflectCamera );
	glPopMatrix();
	delete tr;

	// Floor tiles
	map->displayTiles();

	// Top of map
	tr = new TransparentRenderer();
	glUseProgram(renderBloom?bikeBrightProgram:bikeProgram);
	player1->display(tr, cameraPos);
	player2->display(tr, cameraPos);
	glUseProgram(renderBloom ? brightProgram:0);
	tr->display( cameraPos );
	delete tr;
}
