#ifndef INC_ENGINE_H
#define INC_ENGINE_H

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
  void fill_white();
  SDL_Surface* get();
  
 private:
  Screen(); // ctor hidden
  //  Screen(Screen const&); // copy ctor hidden
  //  Screen& operator=(Screen const&); // assign op. hidden
  ~Screen(); // dtor hidden
  static SDL_Surface *screen;
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
  //Collision tests
  //I'm sure this ENUM used like this is bad or wrong... FIX ME
  static enum {COLLISION_NULL, COLLISION_TRUE, COLLISION_BLIMIT} COLLISION_TYPE;
  static  Uint32 is_collided (cGameObject *objectA, cGameObject *objectB);
};

class cGameObject {
 private:
  SDL_Rect bbox;
 protected:
  Sint32 velx, vely, accelx, accely;
  SDL_Rect position;
  SDL_Rect oldposition;

 public:
  cGameObject(Uint32 = 0, Uint32 = 0, Uint32 = 0, Uint32 = 0);
  //  ~cGameObject();
  void set_accel(const Sint32 &newaccelx, const Sint32 &newaccely);
  void read_accel(Sint32& read_accelx, Sint32& read_accely) const;
  void set_vel(const Sint32 &newvelx, const Sint32 &newvely);
  void read_vel(Sint32& read_velx, Sint32& read_vely) const;
  void set_pos(const Uint32 &newposx, const Uint32 &newposy);
  void read_pos(Uint32& read_posx, Uint32& read_posy) const;
  virtual void read_dims(Uint32 &read_dimx, Uint32 &read_dimy) const;
  void move();
  void del_move();
};

#endif  /* INC_ENGINE_H */
