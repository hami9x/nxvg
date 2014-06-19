#include <iostream>
#include "nxvg.h"
#include <SDL2/SDL.h>

using std::cout;

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO); // Init SDL2

    // Create a window. Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    SDL_Window *window = SDL_CreateWindow(
    "NXVG demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
    );

    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glcontext);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != 0)
    {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return 1;
    }


    // Now, regular OpenGL functions ...
    glMatrixMode(GL_PROJECTION|GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, 1024, 768);

    void *nx = nxvgNew(1024, 768);

    // ... can be used alongside SDL2.
    SDL_Event e;

    glClearColor(1,1,1,1);          // Draw with OpenGL.
    glClear(GL_COLOR_BUFFER_BIT);

//    GLfloat pts2[] = {
//        -0.6f, -0.2f,
//        -0.2f, 0.5f,
//        -0.5f, -0.9f
//    };
//
//    int segs2[] = {NX_LINE};
//    nxvgDrawPath(nx, pts2, segs2, 1, nxvgColor(1, 1, 1, 1));


    GLfloat pts[] = {
        -0.2f, -0.2f,
        0.f, 0.5f,
        0.3f, -0.23f,
        0.0f, 0.3f,
        -0.2f, -0.3f
    };

    int segs[] = {NX_QUADRATIC, NX_QUADRATIC};
    nxvgDrawPath(nx, pts, segs, 2, nxvgColor(1, 0, 0, 1));

    while(e.type!=SDL_KEYDOWN && e.type!=SDL_QUIT){  // Enter main loop.

        SDL_PollEvent(&e);      // Check for events.

        SDL_GL_SwapWindow(window);  // Swap the window/buffer to display the result.
        SDL_Delay(10);              // Pause briefly before moving on to the next cycle.
    }

    nxvgDelete(nx);
    // Once finished with OpenGL functions, the SDL_GLContext can be deleted.
    SDL_GL_DeleteContext(glcontext);

    // Done! Close the window, clean-up and exit the program.
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
