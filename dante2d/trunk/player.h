#ifndef INC_PLAYER_H
#define INC_PLAYER_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "sprite.h"
#include "control.h"
#include "world.h"
#include "collision.h"

class cGameObject;
class PlayerClass;

//Refactor, move state class
class AbstractStateClass 
{
 public:
  virtual bool nextState(PlayerClass* player) = 0;
  virtual void setMovement(PlayerClass* player) = 0;
  virtual void setAnimation(PlayerClass* player) = 0;
 private:
  virtual void check_events() = 0;
};
class StateFalling : public AbstractStateClass 
{
 public:
  StateFalling();
  bool nextState(PlayerClass* player);
  void setMovement(PlayerClass* player);
  void setAnimation(PlayerClass* player);
 private:
  Control *shoot;
  Sint32 FALL_SPEED, FALL_ACCEL;
  void check_events();
};

class StateJumpShooting : public AbstractStateClass 
{
 public:
  StateJumpShooting();
  bool nextState(PlayerClass* player);
  void setMovement(PlayerClass* player);
  void setAnimation(PlayerClass* player);
 private:
  Control *shoot;
  Sint32 JUMP_SPEED, JUMP_ACCEL;
  void check_events();
};

class StateJumping : public AbstractStateClass 
{
 public:
  StateJumping();
  bool nextState(PlayerClass* player);
  void setMovement(PlayerClass* player);
  void setAnimation(PlayerClass* player);
 private:
  Control *shoot, *jump;
  Sint32 JUMP_SPEED, JUMP_ACCEL, JUMP_ACCEL_SMALL;
  void check_events();
};

class StateRunRight : public AbstractStateClass 
{
 public:
  StateRunRight();
  //  ~StateRunRIght()
  bool nextState(PlayerClass* player);
  void setMovement(PlayerClass* player);
  void setAnimation(PlayerClass* player);
 private:
  Control *right, *left;
  int XRUN_SPEED;
  void check_events();
};

class StateRunLeft : public AbstractStateClass 
{

 public:
  StateRunLeft();
  //  ~StateRunRIght()
  bool nextState(PlayerClass* player);
  void setMovement(PlayerClass* player);
  void setAnimation(PlayerClass* player);
 private:
  Control *left, *right;
  int YRUN_SPEED;
  void check_events();
};

class StateStanding : public AbstractStateClass 
{
 public:
  //  StateRunRight();
  //  ~StateRunRIght()
  bool nextState(PlayerClass* player);
  void setMovement(PlayerClass* player);
  void setAnimation(PlayerClass* player);
 private:
  void check_events();
};

class PlayerClass : public cGameObject
{
  //Friend classes
  friend class StateJumping;
  friend class StateRunRight;
  friend class StateRunLeft;
  friend class StateStanding;
  friend class StateJumpShooting;
  friend class StateFalling;
  
 private:
  //Animation Sprites
  animSprite *dante_runright;
  animSprite *dante_runleft;
  animSprite *dante_standing;
  animSprite *dante_jumping;
  animSprite *dante_jumpshooting;
  Sint32 world_y_max;
  Sint32 world_x_max;
 public:
  //Player Pointer Globals
  AbstractStateClass *currentState;
  animSprite *currentAnimation;

  //Constructor/Destructor
  PlayerClass(Sint16 = 0, Sint16 = 0);
  ~PlayerClass();

  void draw();
  void logic();
  void read_dims(Sint16 &read_dimx, Sint16 &read_dimy) const;
  void move();
};
#endif  /* INC_PLAYER_H */
