#include "sprite.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "assert.h"
#include "engine.h"

using namespace std;

/////////////////////////////////
// cSprite Class defintions
/////////////////////////////////

/**
 * cSprite constructor, currently loads an image, sets the position
 */
cSprite::cSprite(string img_name, Uint32 posx, Uint32 posy)
  : cGameObject(posx, posy)
{
  assert(&img_name != 0);
  string img_path = img_name + ".png";
  img=NULL;

  s_colorkey.r = 0xF1; //0xFF for transparent white
  s_colorkey.g = 0xFF;
  s_colorkey.b = 0xFF;

  //The image that's loaded 
  img = Engine::load_image(img_path);

  //Map the color key 
  Uint32 colorkey = SDL_MapRGB( img->format, s_colorkey.r, s_colorkey.g, s_colorkey.b );
  //Set all pixels of color R 0xFF, G 0xFF, B 0xFF to be transparent 
  SDL_SetColorKey( img, SDL_SRCCOLORKEY, colorkey ); 
}
/**
 * cSprite destructor, frees up image surfaces
 */
cSprite::~cSprite(){
  if(img != NULL)
   SDL_FreeSurface(img);
}
/**
 * draws the sprite to the screen
 */
void cSprite::draw(SDL_Surface *dest)
{
  assert(img != 0);
  SDL_BlitSurface(img, NULL, dest, &position);
}

void cSprite::read_dims(Uint32 &read_dimx, Uint32 &read_dimy) const {
  read_dimx = img->w;
  read_dimy = img->h;
}


/////////////////////////////////
// animSprite Class defintions
/////////////////////////////////

//Public functions
animSprite::animSprite(string img_name, int posx, int posy)
  :cSprite(img_name, posx, posy), anim_time(0), frame_delay(0),
   total_frames(0)
{
  //procees settings file FIX: refactor into function, add "readable" error messages
  if(!read_settings(img_name)) {
    cout << "ERR:There was a problem reading " << img_name << " settings file" << endl;
  }

  assert(anim_time != 0);
  assert(total_frames != 0);
  
  //Check threshold
  //frames per second translated to frames per tenth of second
  int threshold = Engine::framespersecond/10; 
  if (threshold < (total_frames/anim_time)) {
      cout << "Invalid sprite animation time/frames for fps of Engine: -> " 
	   << img_name << endl;
      assert(false);
    }
    //integer devision
    frame_delay = (threshold*anim_time)/total_frames;

    //Set our current frame to the first frame
    currentFrame = &(*frame_vec.begin());
    //Set our frame iterator to the first frame
    frame_iter = frame_vec.begin();
    //Initialize our frame countdown
    frame_countdown = frame_delay;

}

animSprite::~animSprite() {}

void animSprite::draw(SDL_Surface *dest) {
  SDL_BlitSurface(img, currentFrame, dest, &position);
  next_frame();
}

void animSprite::draw(SDL_Surface *dest, SDL_Rect &new_pos) {
  assert(img != 0);
  SDL_BlitSurface(img, currentFrame, dest, &new_pos);
  next_frame();
}

//Private functions
void animSprite::next_frame() {
  frame_countdown--;
  if (frame_countdown == 0) {
    frame_countdown = frame_delay;
    advance_frame();
  } 
}
void animSprite::advance_frame() {
  frame_iter++;
  if (frame_iter == frame_vec.end()) {
    frame_iter = frame_vec.begin();
  }
  currentFrame = &(*frame_iter);
}

//Reset the animation to the first frame
void animSprite::reset_frame() {
    frame_iter = frame_vec.begin();
    frame_countdown = frame_delay;
}

bool animSprite::read_settings(const string &img_name) {

  string img_settings = img_name + ".settings";
  ifstream settings_file(img_settings.c_str());
  string line;

  if(!settings_file) {
    return false;
  }


  if(getline(settings_file, line)) {
    istringstream stream(line);
    stream >> total_frames >> anim_time;;
    if (!stream)
      return false;
  }

  while(getline(settings_file, line)) {
    istringstream stream(line);
    SDL_Rect clip;
    stream >> clip.x >> clip.y >> clip.w >> clip.h;
    if(!stream)
      return false;

    frame_vec.push_back(clip);
  }
  settings_file.close();
  return true;
}

void animSprite::read_dims(Uint32 &read_dimx, Uint32 &read_dimy) const {
  read_dimx = currentFrame->w;
  read_dimy = currentFrame->h;
}
