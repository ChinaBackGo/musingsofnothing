#ifndef INC_ENGINE_H
#define INC_ENGINE_H

/* #ifndef _WINDOWS */
/* #include <windows.h> */
/* #include <SDL/SDL.h> */
/* #include <GL/gl.h> */
/* #endif  /\* _WINDOWS *\/ */

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

//A Meyers Singleton Screen
class Screen {
 public:
  static Screen& Instance() {
    static Screen theScreen;
    return theScreen;
  }
  /* more (non-static) functions here */
  void update();
  void clear();
  void render();
  void reshape();
  void read_cam(float&, float&, float&) const;
  SDL_Surface* get();
    
 private:
  Screen(); // ctor hidden
  //  Screen(Screen const&); // copy ctor hidden
  //  Screen& operator=(Screen const&); // assign op. hidden
  ~Screen(); // dtor hidden
  static SDL_Surface *screen;
  float cam_z;
  float cam_x;
  float cam_y;
};

//General timer class
class Timer {
  Uint32 start_time;

 public:
  Timer();
  void start();
  Uint32 get_elapsed() const;
};

class cGameObject;

//Global engine functions
class Engine {
 public:
  static const Uint32 framespersecond;
  static const Uint32 SCREEN_W;
  static const Uint32 SCREEN_H;
  static const Uint32 SCREEN_B;

  //Functions
  static SDL_Surface* load_image(const std::string& img_path);
  static void init();
  //Timers
  static Uint32 framecap_callbackfunc(Uint32 interval, void *param);

};

class cGameObject {
 private:

 protected:
  SDL_Rect bbox;
  Sint16 velx, vely, accelx, accely;
  Sint32 world_x, world_y;
  SDL_Rect position;
  SDL_Rect oldposition;

 public:
  cGameObject(Sint16 = 0, Sint16 = 0, Sint16 = 0, Sint16 = 0);
  //  ~cGameObject();
  void set_accel(const Sint16 &newaccelx, const Sint16 &newaccely);
  void read_accel(Sint16& read_accelx, Sint16& read_accely) const;
  void set_vel(const Sint16 &newvelx, const Sint16 &newvely);
  void read_vel(Sint16& read_velx, Sint16& read_vely) const;
  void set_pos(const Sint16 &newposx, const Sint16 &newposy);
  void read_pos(Sint16& read_posx, Sint16& read_posy) const;
  void read_world_pos(Sint32& read_posx, Sint32& read_posy) const;
  void set_world_pos(const Sint32 &newposx, const Sint32 &newposy);
  virtual void read_dims(Sint16 &read_dimx, Sint16 &read_dimy) const;
  virtual void move();
  void del_move();
  virtual void draw();
  virtual void print() const;
};

#endif  /* INC_ENGINE_H */
