#include "Gameplay.h"

Gameplay::Gameplay() {
	char model_name[] = "bike.obj";
	char tex_name[] = "bike_texture.bmp";
	char body_name[] = "body.obj";
	char body_tex[] = "body_texture.bmp";
	bike = new Model(model_name,tex_name);
	bike->append(body_name,body_tex);

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

	// set up programs

	bikeProgram = glCreateProgram();
	char shader_name[] = "Shaders/bike.frag";
	int c = CreateShader(GL_FRAGMENT_SHADER, shader_name);
	glAttachShader(bikeProgram,c);
	char shader_name2[] = "Shaders/bike.vert";
	int c2 = CreateShader(GL_VERTEX_SHADER, shader_name2);
	glAttachShader(bikeProgram,c2);
	glLinkProgram(bikeProgram);
	PrintProgramLog(bikeProgram);

	blurProgram = glCreateProgram();
	char blurShaderFrag[] = "Shaders/blur.frag";
	int c3 = CreateShader(GL_FRAGMENT_SHADER, blurShaderFrag);
	glAttachShader(blurProgram,c3);
	char blurShaderVert[] = "Shaders/blur.vert";
	int c6 = CreateShader(GL_VERTEX_SHADER, blurShaderVert);
	glAttachShader(blurProgram,c6);
	glLinkProgram(blurProgram);
	PrintProgramLog(blurProgram);

	blendProgram = glCreateProgram();
	char blendShaderFrag[] = "Shaders/blend.frag";
	int c4 = CreateShader(GL_FRAGMENT_SHADER, blendShaderFrag);
	glAttachShader(blendProgram,c4);
	char blendShaderVert[] = "Shaders/blend.vert";
	int c5 = CreateShader(GL_VERTEX_SHADER, blendShaderVert);
	glAttachShader(blendProgram,c5);
	glLinkProgram(blendProgram);
	PrintProgramLog(blendProgram);

	brightProgram = glCreateProgram();
	char brightShaderFrag[] = "Shaders/bright.frag";
	int c7 = CreateShader(GL_FRAGMENT_SHADER, brightShaderFrag);
	glAttachShader(brightProgram,c7);
	char brightShaderVert[] = "Shaders/bright.vert";
	int c8 = CreateShader(GL_VERTEX_SHADER, brightShaderVert);
	glAttachShader(brightProgram,c8);
	glLinkProgram(brightProgram);
	PrintProgramLog(brightProgram);

	bikeBrightProgram = glCreateProgram();
	char bikeBrightShaderFrag[] = "Shaders/bike_bright.frag";
	int c9 = CreateShader(GL_FRAGMENT_SHADER, bikeBrightShaderFrag);
	glAttachShader(bikeBrightProgram,c9);
	char bikeBrightShaderVert[] = "Shaders/bike_bright.vert";
	int c10 = CreateShader(GL_VERTEX_SHADER, bikeBrightShaderVert);
	glAttachShader(bikeBrightProgram,c10);
	glLinkProgram(bikeBrightProgram);
	PrintProgramLog(bikeBrightProgram);


	// Set up texture to write to for rendering the scene

	glGenFramebuffers(1,&frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// generate the two textures
	glGenTextures(1,&renderNormalTexture);
	// set up first output texture
	glBindTexture(GL_TEXTURE_2D, renderNormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


	// attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderNormalTexture, 0);
	glDrawBuffers(1, colorAttachments);



	// Set up texture to write to for rendering the glowing parts of the scene

	glGenFramebuffers(1,&blurBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer);
	// generate the two textures
	glGenTextures(1,&blurTexture);
	// set up first output texture
	glBindTexture(GL_TEXTURE_2D, blurTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// attach texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);
	glDrawBuffers(1, colorAttachments);


	

	// generate textures/ frame buffers for ping pong gaussian blur
	glGenFramebuffers(2, pingpongFrameBuffer);
	glGenTextures(2,pingpongTextures);

	// set up 1st frame buffer and texture for gaussian blur
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[0]);
	glBindTexture(GL_TEXTURE_2D, pingpongTextures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextures[0],0);
	glDrawBuffers(1,colorAttachments);




	// set up 2nd frame buffer and texture for gaussian blur
	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[1]);
	glBindTexture(GL_TEXTURE_2D, pingpongTextures[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	//set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTextures[1],0);
	glDrawBuffers(1,colorAttachments);


	// find locations of variables in programs
	horizontalLocation = glGetUniformLocation(blurProgram, "horizontal");
	bloomBoolLoc = glGetUniformLocation(blendProgram, "bloom");
	exposureLoc = glGetUniformLocation(blendProgram, "exposure");
	widthLoc = glGetUniformLocation(blurProgram, "width");
	heightLoc = glGetUniformLocation(blurProgram, "height");
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

void Gameplay::special(int key, int x, int y) { /*NEEDED in GLUT, maybe different in other system */

}

void Gameplay::mouse(/*TODO*/) {
	
}

void Gameplay::update() {
	double tDiff = getElapsedTime();

	th += (thup-thdown)*50*tDiff;
	ph += (phup-phdown)*50*tDiff;

	if( state == STATE_PLAYING ) {
		player1->movePlayer(tDiff);
		player2->movePlayer(tDiff);

		if( testCollision( player1->getHitbox(), player2->getHitbox() ) ) {
			state = STATE_TIE;
			player1->setAlive(false);
			player2->setAlive(false);
		}
		if( player1->getTrail()->testTrailHit( player2->getHitbox() ) || player2->getTrail()->testTrailHit( player2->getHitbox() ) || map->testWallHits( player2->getHitbox() ) ) {
			state = STATE_P1_WIN;
			player2->setAlive(false);
		}
		if( player2->getTrail()->testTrailHit( player1->getHitbox() ) || player1->getTrail()->testTrailHit( player1->getHitbox() ) || map->testWallHits( player1->getHitbox() ) ) {
			if( state == STATE_P1_WIN )
				state = STATE_TIE;
			else
				state = STATE_P2_WIN;
			player1->setAlive(false);
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



void Gameplay::render2DScreen()
{
	// switch to 2D displaying
	int vPort[4];
	glGetIntegerv(GL_VIEWPORT, vPort);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
   	glLoadIdentity();

   //display texture
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0,0);
	glVertex2d(0,0);
	glTexCoord2f(1,0);
	glVertex2d(wWidth,0);
	glTexCoord2f(1,1);
	glVertex2d(wWidth,wHeight);

	glTexCoord2f(1,1);
	glVertex2d(wWidth,wHeight);
	glTexCoord2f(0,1);
	glVertex2d(0,wHeight);
	glTexCoord2f(0,0);
	glVertex2d(0,0);
	glEnd();
	
	//go back to 3d
	glMatrixMode(GL_PROJECTION);
   	glPopMatrix();   
   	glMatrixMode(GL_MODELVIEW);
   	glPopMatrix();
}


void Gameplay::postProcessingSetup()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Gameplay::postProcessingStep2()
{
	
	glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

}

void Gameplay::postProcessing()
{
	glViewport(0, 0, wWidth, wHeight);
	//glUniform1f(glGetUniformLocation(brightProgram, "threshold"),bloomThreshold);
	//printf("\rBloom Threshold: %f", bloomThreshold);

	glBindFramebuffer(GL_FRAMEBUFFER, blurBuffer);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderNormalTexture);
	glUseProgram(brightProgram);
	render2DScreen();



	glDisable( GL_DEPTH_TEST );
	bool horizontal = true, first_iteration = true;
	int amount = NUMBER_GAUSSIAN_PASSES;
	glUseProgram(blurProgram);
	glEnable(GL_TEXTURE_2D);
	//printf("blurring\n");
	for (unsigned int i = 0; i < amount; i++)
	{

	    glBindFramebuffer(GL_FRAMEBUFFER, pingpongFrameBuffer[horizontal]); 
	    glViewport(0, 0, wWidth, wHeight);
	    glActiveTexture(GL_TEXTURE0);
	    glUniform1ui(horizontalLocation, horizontal);
	    //glUniform1i(SCREEN_WIDTH, widthLoc);
	    //glUniform1i(SCREEN_HEIGHT, heightLoc);
	    glBindTexture(GL_TEXTURE_2D, first_iteration ? blurTexture : pingpongTextures[!horizontal]); 
	    
	    render2DScreen();
	    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	    
	    horizontal = !horizontal;
	    if (first_iteration)
	        first_iteration = false;
	}
 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
	//printf("displaying\n");




	glUseProgram(blendProgram);
	glUniform1ui(bloomBoolLoc, bloomOn);
	//printf("Bloom: %s\n", bloomOn ? "True":"False");
	glUniform1f(exposureLoc, exposure);

	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);//:GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glUniform1i(glGetUniformLocation(blendProgram, "scene"),0);
	glUniform1i(glGetUniformLocation(blendProgram, "bloomBlur"),1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderNormalTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongTextures[0]);

	render2DScreen();

	glDisable(GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable( GL_DEPTH_TEST );
	glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
	glActiveTexture(GL_TEXTURE0);
}