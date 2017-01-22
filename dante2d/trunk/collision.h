#ifndef INC_COLLISION_H
#define INC_COLLISION_H
#include <string>
#include <iostream>
#include <typeinfo>
#include <vector>
#include "assert.h"
#include "SDL/SDL.h"
#include "engine.h"
#include "player.h"
#include "world.h"

class Collision
{
 protected:
 public:
  //Collision tests
  //I'm sure this ENUM used like this is bad or wrong... FIX ME
  static enum {COLLISION_NULL, COLLISION_TRUE, COLLISION_BLIMIT} COLLISION_TYPE;
  static Uint32 is_collided (cGameObject *objectA, cGameObject *objectB);
  static Uint32 obj_collision (cGameObject *objectA, cGameObject *objectB);
  static Uint32 map_collision (cGameObject *objectA);
};

/* class DamageBox : public CollisionBox */
/* { */
/* private: */
/*   SDL_Rect collision_box; */
  
/*  public: */
  
/* }; */

#endif  /* INC_COLLISION_H */
