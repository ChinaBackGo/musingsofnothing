#include "engine.h"
#include <iostream>
#include <typeinfo>
#include <GL/gl.h>
#include "assert.h"
#include "sprite.h"
#include "player.h"
using namespace std;

SDL_Surface *Screen::screen;
const Uint32 Engine::framespersecond=30;
const Uint32 Engine::SCREEN_W = 1280;
const Uint32 Engine::SCREEN_H = 720;
const Uint32 Engine::SCREEN_B = 32;

///////////////////////////////
// Screen Singleton functions
//////////////////////////////

Screen::Screen() {
  //Create a OpenGL  window (16:9 ratio)
  screen = SDL_SetVideoMode(Engine::SCREEN_W, Engine::SCREEN_H, 
			    Engine::SCREEN_B, SDL_OPENGL); 
  
  if(!screen){ //Couldn't create window?
    std::cerr << "Couldn't create screen"; //Output to stderr and quit
    exit(1);
  }
}
Screen::~Screen() {
  SDL_FreeSurface( screen );
  //std::cout << "screen destroyed!\n";
}

//Deprecated now, from SDL rendering
SDL_Surface* Screen::get() {
  return screen;
}
void Screen::update() {
  //Update the screen
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL_GL_SwapBuffers();

  //Poor test, fix this.
  if (glGetError() != GL_NO_ERROR)
    std::cout << "GL ERROR!" << std::endl;

}

void Screen::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::render() {
  clear();

  //Move Camera

  //Render functions here

  update();
}

void Screen::reshape()
{
  int height = screen->h;
  int width = screen->w;

  float cam_near;
  float cam_far;

  cam_near = 1.0;
  //Tan (theta) = O/A, we set width ratio to 2 lenghts, so O = 1.0
  //A = 1.0 or cam_near
  //Could calculate as TAN(theta) = Width ration/2 / cam_near, no point, this is fixed
  //And simplfies following equations.
  
  cam_z = ((float)width)/2;

  //This is just set to double the distance
  cam_far = cam_z*2;

  //Set to camera position
  cam_z *= -1;

  GLfloat h = (GLfloat) height / (GLfloat) width;
  cam_x = (GLfloat)width/2 * -1;
  cam_y = (GLfloat)height/2 * -1;

     glViewport(0, 0, (GLint) width, (GLint) height);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glFrustum(-1.0, 1.0, -h, h, cam_near, cam_far);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glTranslatef(cam_x, cam_y, cam_z);
}

void Screen::read_cam(float &new_cam_x, float &new_cam_y, float &new_cam_z) const {
  new_cam_x = cam_x;
  new_cam_y = cam_y;
  new_cam_z = cam_z;

}


//FIX ME
SDL_Surface* Engine::load_image(const std::string& img_path) {
  //The image that's loaded 
  SDL_Surface* loadedImage = NULL; //The optimized image that will be used
  loadedImage = IMG_Load( img_path.c_str() );
  if (!loadedImage) {
      std::cout << "Image:" + img_path + " could not be loaded\n";
      std::cout << "Image Error:" << IMG_GetError();
      assert(SDL_TRUE);
      // handle error
      }
  return loadedImage;
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
  //Sets up the screen dimensions
  Screen &initscreen =  Screen::Instance();
  //Sets Viewport
  initscreen.reshape();

  //GL enables
  //  glEnable(GL_TEXTURE_2D);

  //Magenta temprorary
  glClearColor(1.0,0.4,1.0,0.0);

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


////////////////////////////////
//cGameObject functions
///////////////////////////////
cGameObject::cGameObject(Sint16 posx, Sint16 posy, Sint16 bbw, Sint16 bbh)
  : velx(0), vely(0), accelx(0), accely(0), world_x(0), world_y(0)
{
  position.x = posx;
  position.y = posy;
  oldposition.x = posx;
  oldposition.y = posy;

  bbox.x = posx;
  bbox.y = posy;
  bbox.w = bbw;
  bbox.h = bbh;
  
}

void cGameObject::set_accel(const Sint16 &newaccelx, const Sint16 &newaccely) {
  accelx = newaccelx;
  accely = newaccely;
}

void cGameObject::read_accel(Sint16 &read_accelx, Sint16 &read_accely) const {
  read_accelx = accelx;
  read_accely = accely;
}

void cGameObject::set_vel(const Sint16 &newvelx, const Sint16 &newvely) {
  velx = newvelx;
  vely = newvely;
}

void cGameObject::read_vel(Sint16 &read_velx, Sint16 &read_vely) const {
  read_velx = velx;
  read_vely = vely;
}

void cGameObject::set_pos(const Sint16 &newposx, const Sint16 &newposy) {
  position.x = newposx;
  position.y = newposy;
}

void cGameObject::read_pos(Sint16 &read_posx, Sint16 &read_posy) const {
  read_posx = position.x;
  read_posy = position.y;
}
void cGameObject::read_world_pos(Sint32& read_posx, Sint32& read_posy) const {
  read_posx = world_x;
  read_posy = world_y;
}

void cGameObject::set_world_pos(const Sint32 &newposx, const Sint32 &newposy) {
  world_x = newposx;
  world_y = newposy;
}

//Virtual read dimensions function, for game object, game object has a "bounding box"
//since they exist on their own...may make a separate Bounding box class....
void cGameObject::read_dims(Sint16 &read_dimx, Sint16 &read_dimy) const {
  assert(bbox.w > 0);
  assert(bbox.h > 0);
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

void cGameObject::draw() {
  //nothing
  assert(false);
}

void cGameObject::print() const {
  cout << "cGameObject Info:" << endl;
  cout << "Dims: w/h " << bbox.w << " " << bbox.h << endl;
  cout << "Vel: x/y " << velx << " " << vely << endl;
  cout << "Accel: x/y " << accelx << " " << accely << endl;
  cout << "World pos: x/y " << world_x << " " << world_y << endl;
  cout << "Screen pos: x/y " << position.x << " " << position.y << endl;
}

