#ifndef INC_SPRITE_H
#define INC_SPRITE_H

#include <string>
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "engine.h"

class cGameObject;

class cSprite : public cGameObject
{
 private:
  struct st_colorkey {
    Uint8 r;
    Uint8 g;
    Uint8 b;
  };
  st_colorkey s_colorkey;

 protected:
  SDL_Surface *img;
  //Position
 public:
  cSprite(std::string img_name, Uint32 posx = 0, Uint32 posy = 0);
  ~cSprite();

  virtual void draw(SDL_Surface *dest);
  virtual void read_dims(Uint32 &read_dimx, Uint32 &read_dimy) const;
 };

class animSprite : public cSprite 
{
 private:
  std::vector<SDL_Rect> frame_vec;
  std::vector<SDL_Rect>::iterator frame_iter;
  int anim_time; //1/10th seconds
  int frame_delay;
  Uint32 frame_countdown;
  int total_frames; //may not be required?
  SDL_Rect *currentFrame;
  void next_frame();
  void advance_frame();
  bool read_settings(const std::string &img_name);

 public:
  animSprite(std::string img_name, int posx = 0, int posy = 0);
  ~animSprite();
  virtual void draw(SDL_Surface *dest);
  virtual void draw(SDL_Surface *dest, SDL_Rect& new_pos);
  void reset_frame();
  virtual void read_dims(Uint32 &read_dimx, Uint32 &read_dimy) const;
};

#endif  /* INC_SPRITE_H */
