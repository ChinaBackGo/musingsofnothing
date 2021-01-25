#ifndef INC_SPRITE_H
#define INC_SPRITE_H

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "engine.h"
#include "texture.h"

class animSprite : public cGameObject
{
 private:
  std::vector<Texture> frame_vec;
  std::vector<Texture>::iterator frame_iter;
  int anim_time; //1/10th seconds
  int frame_delay;
  Uint32 frame_countdown;
  int total_frames; //may not be required?
  Texture *currentFrame;
  void next_frame();
  void advance_frame();
  bool read_settings(const std::string &img_name);

 public:
  animSprite(std::string img_name, Sint16 posx = 0, Sint16 posy = 0);
  ~animSprite();
  virtual void draw();
  void reset_frame();
  virtual void read_dims(Sint16 &read_dimx, Sint16 &read_dimy) const;
};

#endif  /* INC_SPRITE_H */
