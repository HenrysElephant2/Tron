#include "Main.h"



//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

// Window settings
int fov = 55;
double dim = 100.0;
double asp=1;

double ph = 0;      //  Elevation of view angle
double th = 0;      //  Azimuth of view angle


// Booleans to control window movement
bool thup = false;
bool thdown = false;
bool phup = false;
bool phdown = false;

// Bools to move hitbox
bool hup = false;
bool hdown = false;
bool hleft = false;
bool hright = false;
bool hforward = false;
bool hbackward = false;

// Bools to rotate hitbox
bool pu = false;
bool pd = false;
bool yl = false;
bool yr = false;
bool rr = false;
bool rl = false;

//Main loop flag
bool quit = false;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

Vector p1(1,-1,0);
Vector f1(0,0,-1);
Vector u1(0,1,0);
Hitbox h1(p1, f1, u1, 2, 2, 2);

Vector p2(-3,-5,0);
Vector f2(0,0,-1);
Vector u2(0,1,0);
Hitbox h2(p2, f2, u2, 2, 2, 2);

bool colliding = testCollision( h1, h2 );

// Modified Project function from ex9
static void Project()
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    gluPerspective(fov,asp,dim/6,6*dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
}

// reshape function from ex9
void reshape(int width,int height) {
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project();
}


bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else {
        //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

        //Create window
        gWindow = SDL_CreateWindow( "Tron", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else {
            SDL_SetWindowResizable(gWindow, SDL_TRUE); // Allow resizable window

            //Create context
            gContext = SDL_GL_CreateContext( gWindow );
            if( gContext == NULL ) {
                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else {
                //Use Vsync
                if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
                    printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
                }

                //Initialize OpenGL
                if( !initGL() ) {
                    printf( "Unable to initialize OpenGL!\n" );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool initGL() {
    bool success = true;
    GLenum error = GL_NO_ERROR;

    Project();
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR ) {
        success = false;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR ) {
        success = false;
    }
    
    //Initialize clear color
    glClearColor( 0, 0, 0, 1 );
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR ) {
        success = false;
    }
    
    return success;
}

void keyDown( SDL_Keycode key, int x, int y ) {
    switch( key ) {
        case SDLK_q: quit = true; break;
        case SDLK_w: phup = true; break;
        case SDLK_s: phdown = true; break;
        case SDLK_a: thdown = true; break;
        case SDLK_d: thup = true;break;
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

void keyUp( SDL_Keycode key, int x, int y ) {
    switch( key ) {
        case SDLK_w: phup = false; break;
        case SDLK_s: phdown = false; break;
        case SDLK_a: thdown = false; break;
        case SDLK_d: thup = false;break;
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

void update() {
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
        h1.pitch(-rSpeed);
    if( pd && !pu )
        h1.pitch(rSpeed);
    if( yl && !yr )
        h1.yaw(rSpeed);
    if( yr && !yl )
        h1.yaw(-rSpeed);
    if( rl && !rr )
        h1.roll(-rSpeed);
    if( rr && !rl )
        h1.roll(rSpeed);

    if( hforward )
        h1.move( Vector(0,0,-mSpeed) );
    if( hbackward )
        h1.move( Vector(0,0,mSpeed) );
    if( hleft )
        h1.move( Vector(-mSpeed,0,0) );
    if( hright )
        h1.move( Vector(mSpeed,0,0) );
    if( hup )
        h1.move( Vector(0,mSpeed,0) );
    if( hdown )
        h1.move( Vector(0,-mSpeed,0) );
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    // Set eye position
    double Ex = -.3*dim*Sin(th)*Cos(ph);
    double Ey = +.3*dim  *Sin(ph);
    double Ez = +.3*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    
    h1.renderSelf(colliding);
    h2.renderSelf(colliding);

    // glFlush();
    // glSwapBuffers();
}

void close() {
    //Destroy window    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] ) {
    //Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        //Event handler
        SDL_Event e;
        
        //Enable text input
        SDL_StartTextInput();

        //While application is running
        while( !quit ) {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 ) {
                //User requests quit
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                //Handle keypress with current mouse position
                else if( e.type == SDL_KEYDOWN ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    SDL_Keycode key = e.key.keysym.sym;
                    keyDown( key, x, y );
                }
                //Handle keypress with current mouse position
                else if( e.type == SDL_KEYUP ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    SDL_Keycode key = e.key.keysym.sym;
                    keyUp( key, x, y );
                }
                else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
                    reshape(e.window.data1, e.window.data2);
                }
            }

            //Update 
            update();
            //Render quad
            render();
            
            //Update screen
            SDL_GL_SwapWindow( gWindow );
        }
        
        //Disable text input
        SDL_StopTextInput();
    }

    //Free resources and close SDL
    close();

    return 0;
}