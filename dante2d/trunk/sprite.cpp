#include "sprite.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "assert.h"
#include "engine.h"

using namespace std;

/////////////////////////////////
// animSprite Class defintions
/////////////////////////////////

//Public functions
animSprite::animSprite(string img_name, Sint16 posx, Sint16 posy)
  :cGameObject(posx, posy), anim_time(0), frame_delay(0),
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

animSprite::~animSprite() {
  vector<Texture>::iterator iter;
  //This could be foreach??
  for (iter = frame_vec.begin(); iter != frame_vec.end(); ++iter) {
    iter->delete_tex();
  }
}



void animSprite::draw() {
  currentFrame->set_pos(position.x, position.y);
  currentFrame->draw();
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

//Completely rewrite
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
  settings_file.close();

  assert (total_frames > 0);
  assert (anim_time > 0);

  //Load up the texture frames!
  for (int i = 0; i < total_frames; i++) {
    ostringstream tex_name;
    
    if (i < 10)
      tex_name << img_name << "0" << i << ".png";
    else
      tex_name << img_name << i << ".png";
    
    Texture new_tex(tex_name.str());
    frame_vec.push_back(new_tex);
  }

  return true;
}

void animSprite::read_dims(Sint16 &read_dimx, Sint16 &read_dimy) const {
  currentFrame->read_dims(read_dimx, read_dimy);
}
