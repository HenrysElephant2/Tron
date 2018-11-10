#include "Main.h"



//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Window settings
int fov = 55;
double dim = 100.0;
double asp=1;

double ph = 0;          //  Elevation of view angle
double th = 0;          //  Azimuth of view angle


//Main loop flag
bool quit = false;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;


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

void handleKeys( unsigned char key, int x, int y ) {
    if( key == 'q' ) {
        quit = true;
    }
}

void update() {
    th += 5;
    render();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();

    // Set eye position
    double Ex = -.2*dim*Sin(th)*Cos(ph);
    double Ey = +.2*dim        *Sin(ph);
    double Ez = +.2*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    
    //Render quad
    glRotatef(0.4f,0.0f,1.0f,0.0f);    // Rotate The cube around the Y axis
    glRotatef(0.2f,1.0f,1.0f,1.0f);
    glColor3f(0.0f,1.0f,0.0f); 

    glBegin( GL_QUADS );
        glVertex3d( -0.5, -0.5, 0 );
        glVertex3d( 0.5, -0.5, 0 );
        glVertex3d( 0.5, 0.5, 0 );
        glVertex3d( -0.5, 0.5, 0 );
    glEnd();

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
                else if( e.type == SDL_TEXTINPUT ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    handleKeys( e.text.text[ 0 ], x, y );
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