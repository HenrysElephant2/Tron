#include "Main.h"

//Screen dimension constants
static int SCREEN_WIDTH = 1400;
static int SCREEN_HEIGHT = 900;

// Window settings
int fov = 55;
double asp=(double)SCREEN_WIDTH/SCREEN_HEIGHT;

//Main loop flag
bool quit = false;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;


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

            // Create context
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
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR ) {
        std::cout << 1 << std::endl;
        success = false;
    }

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR ) {
        std::cout << 2 << std::endl;
        success = false;
    }
    
    //Initialize clear color
    glClearColor( 0, 0, 0, 1 );
    
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR ) {
        std::cout << 31 << std::endl;
        success = false;
    }
    
    return success;
}


void close() {
    //Destroy window    
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool testQuit( SDL_Keycode key ) {
    return key == SDLK_q;
}

int main( int argc, char* args[] ) {
    // Start up SDL and create window
    if( !init() ) {
        printf( "Failed to initialize!\n" );
    }
    else {
        // Initialize the state to hitbox test
        GameState *currentState = new Menu();
        currentState->reshape(SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_Event e;
        SDL_StartTextInput();

        while( !quit ) {
            while( SDL_PollEvent( &e ) != 0 ) {
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                else if( e.type == SDL_KEYDOWN ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    SDL_Keycode key = e.key.keysym.sym;
                    currentState->keyDown( key, x, y );
                    quit = testQuit(key);
                }
                else if( e.type == SDL_KEYUP ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    SDL_Keycode key = e.key.keysym.sym;
                    currentState->keyUp( key, x, y );
                }
                else if( e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED ) {
                    SCREEN_WIDTH = e.window.data1;
                    SCREEN_HEIGHT = e.window.data2;
                    currentState->reshape(SCREEN_WIDTH, SCREEN_HEIGHT);
                }
                else if( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    currentState->mouseDown( x, y );
                }
                else if( e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT ) {
                    int x = 0, y = 0;
                    SDL_GetMouseState( &x, &y );
                    currentState->mouseUp( x, y );
                }
            }
            currentState->update();
            currentState->display();

            GameState *nextState = currentState->getNextState();
            if( nextState != NULL ) {
                // delete currentState;
                currentState = nextState;
                currentState->reshape(SCREEN_WIDTH, SCREEN_HEIGHT);
            }

            if( currentState->getQuit() )
                quit = true;

            if( glGetError() != GL_NO_ERROR )
                std::cout << "Hurghhhh" << std::endl;
            
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


