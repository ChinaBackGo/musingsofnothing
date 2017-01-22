#include "engine.h"
#include <iostream>
#include <typeinfo>
#include "assert.h"
#include "sprite.h"
#include "player.h"


SDL_Surface *Screen::screen;
const Uint32 Engine::framespersecond=30;
const Uint32 Engine::SCREEN_W = 1024;
const Uint32 Engine::SCREEN_H = 576;
const Uint32 Engine::SCREEN_B = 32;

///////////////////////////////
// Screen Singleton functions
//////////////////////////////

Screen::Screen() {
  screen = SDL_SetVideoMode(Engine::SCREEN_W, Engine::SCREEN_H, 
			    Engine::SCREEN_B, SDL_HWSURFACE); //Create a 1024x576x32bit window (16:9 ratio)
  
  if(!screen){ //Couldn't create window?
    std::cerr << "Couldn't create screen"; //Output to stderr and quit
    exit(1);
  }
}
Screen::~Screen() {
  SDL_FreeSurface( screen );
  //std::cout << "screen destroyed!\n";
}
SDL_Surface* Screen::get() {
  return screen;
}
void Screen::update() {
    //Update the screen
  if( SDL_Flip( screen ) == -1 )
    {
      std::cerr << "Flip error\n";
    }
}

void Screen::fill_white() {
  SDL_FillRect( screen, &screen->clip_rect, 
		SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
}

SDL_Surface* Engine::load_image(const std::string& img_path) {
  //The image that's loaded 
  SDL_Surface* loadedImage = NULL; //The optimized image that will be used 
  SDL_Surface* img = NULL;

  loadedImage = IMG_Load( img_path.c_str() ); 
  //If the image loaded 
  if( loadedImage != NULL ) { //Create an optimized image 
    img = SDL_DisplayFormat( loadedImage ); 
    //Free the old image 
    SDL_FreeSurface( loadedImage ); 
  } else {
    std::cout << "Image: " + img_path + " not found!\n";
    assert(false);
  }
  return img;
}

////////////////////////////////
//Timer Functions
///////////////////////////////
Timer::Timer() {
  start_time = 0;
}
void Timer::start() {
  start_time = SDL_GetTicks();
}
Uint32 Timer::get_elapsed() const{
  return SDL_GetTicks() - start_time;
}

///////////////////////////////
//Engine Functions
//////////////////////////////
void Engine::init() {
  if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
    std::cerr << "Unable to init SDL:" << SDL_GetError() << std::endl;
    exit(1);
  }

  //Set up timers
  SDL_TimerID frame_timerID;
  Uint32 frame_cap = ( (1000/Engine::framespersecond)/10 ) * 10;
  frame_timerID = SDL_AddTimer(frame_cap, Engine::framecap_callbackfunc, 0);
}

Uint32 Engine::framecap_callbackfunc(Uint32 interval, void *param)
{
  SDL_Event event;
  SDL_UserEvent userevent;

  /* In this example, our callback pushes an SDL_USEREVENT event
           into the queue, and causes ourself to be called again at the
           same interval: */

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = NULL;
  userevent.data2 = NULL;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);
  return(interval);
}

Uint32 Engine::is_collided(cGameObject *objectA, cGameObject *objectB) {
  Uint32 AX1, AX2, BX1, BX2, AY1, AY2, BY1, BY2;

      
  //Set our bounding boxes for the objectA
  objectA->read_pos(AX1, AY1);
  objectA->read_dims(AX2, AY2);

  //Read dims only returns dimensions, must add these to X1/X2 to get real rect coords
  AX2 += AX1;
  AY2 += AY1;

  if ( objectB == 0 && (typeid(*objectA) == typeid(PlayerClass)) )
    if (AY2 > Engine::SCREEN_H) {
      //find Y over shoot
      Uint32 playx, playy, playh, playw;
      objectA->read_pos(playx, playy);
      objectA->read_dims(playw, playh);
      Uint32 yovershoot = (playy + playh) - Engine::SCREEN_H;
      playy -= yovershoot;
      std::cout << "overshoot" << yovershoot << std::endl;
      objectA->set_pos(playx, playy);
      return COLLISION_BLIMIT;
    }
      else
	return COLLISION_NULL;

  //Set our bounding boxes for objectB
  objectB->read_pos(BX1, BY1);
  objectB->read_dims(BX2, BY2);

  BX2 += BX1;
  BY2 += BY1;

  //Test horizontally first
  if (AX2 < BX1)
    return COLLISION_NULL;

  if (AX1 > BX2)
    return COLLISION_NULL;

  if (AY1 > BY2)
    return COLLISION_NULL;

  if (AY2 < BY1)
    return COLLISION_NULL;

  return COLLISION_TRUE;

  //Implement TypeID checking for player/animsprite and pixel perfect collision
  
}
////////////////////////////////
//cGameObject functions
///////////////////////////////
cGameObject::cGameObject(Uint32 posx, Uint32 posy, Uint32 bbw, Uint32 bbh)
  : velx(0), vely(0), accelx(0), accely(0)
{
  assert(posx >= 0);
  assert(posy >= 0);
  position.x = posx;
  position.y = posy;
  oldposition.x = posx;
  oldposition.y = posy;

  bbox.x = posx;
  bbox.y = posy;
  bbox.w = bbw;
  bbox.h = bbh;
  
}

void cGameObject::set_accel(const Sint32 &newaccelx, const Sint32 &newaccely) {
  accelx = newaccelx;
  accely = newaccely;
}

void cGameObject::read_accel(Sint32 &read_accelx, Sint32 &read_accely) const {
  read_accelx = accelx;
  read_accely = accely;
}

void cGameObject::set_vel(const Sint32 &newvelx, const Sint32 &newvely) {
  velx = newvelx;
  vely = newvely;
}

void cGameObject::read_vel(Sint32 &read_velx, Sint32 &read_vely) const {
  read_velx = velx;
  read_vely = vely;
}

void cGameObject::set_pos(const Uint32 &newposx, const Uint32 &newposy) {
  position.x = newposx;
  position.y = newposy;
}

void cGameObject::read_pos(Uint32 &read_posx, Uint32 &read_posy) const {
  read_posx = position.x;
  read_posy = position.y;
}

//Virtual read dimensions function, for game object, game object has a "bounding box"
//since they exist on their own...may make a separate Bounding box class....
void cGameObject::read_dims(Uint32 &read_dimx, Uint32 &read_dimy) const {
  read_dimx = bbox.w;
  read_dimy = bbox.h;
}

void cGameObject::move() {
  velx += accelx;
  vely += accely;
  oldposition.x = position.x;
  oldposition.y = position.y;
  position.x += velx;
  position.y += vely;
}

void cGameObject::del_move() {
  position.x = oldposition.x;
  position.y = oldposition.y;
}

