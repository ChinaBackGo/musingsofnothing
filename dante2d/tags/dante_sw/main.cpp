#include<iostream>
#include<string>
#include <cstdlib> // For some useful functions such as atexit :)
#include "SDL/SDL.h" // main SDL header
#include "SDL/SDL_image.h" //SDL image header
#include "event.h"

using namespace std;

void cInit();
SDL_Surface *load_image( std::string filename );
const int SCREEN_HEIGHT = 1024;
const int SCREEN_WIDTH = 576;
const int SCREEN_BITDEPTH = 32;

int main( int argc, char* args[] )
{
  SDL_Surface *screen;
  
  cInit();
  
  //Breaks dynamically loaded libs?  
  atexit(SDL_Quit); //Now that we're enabled, make sure we cleanup
  
  screen = SDL_SetVideoMode(SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_BITDEPTH, SDL_HWSURFACE); //Create a 1024x576x32bit window (16:9 ratio)
  
  if(!screen){ //Couldn't create window?
    std::cerr << "Couldn't create screen"; //Output to stderr and quit
    exit(1);
  }


  //Load an image
  SDL_Surface *sprite;  
  string imgpath= "sprites/dante_run.png";

  sprite = load_image(imgpath);
  SDL_BlitSurface( sprite, NULL, screen, NULL );
  //Update the screen
  if( SDL_Flip( screen ) == -1 )
    {
      return 1;    
    }
  
  while(!(cEvent::s_done)){ //While program isn't done
    cEvent::poll_events();
  } //Program done, exited

}

void cInit ()
{
  cEvent::s_done = false;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }

}

SDL_Surface *load_image( std::string filename ) 
{ 
  //The image that's loaded 
  SDL_Surface* loadedImage = NULL; //The optimized image that will be used 
  SDL_Surface* optimizedImage = NULL; //Load the image using SDL_image 

  loadedImage = IMG_Load( filename.c_str() ); 
  //If the image loaded 
  if( loadedImage != NULL ) { //Create an optimized image 
    optimizedImage = SDL_DisplayFormat( loadedImage ); //Free the old image 
    SDL_FreeSurface( loadedImage ); 
  } 
  //Return the optimized image 
  return optimizedImage; 
}
