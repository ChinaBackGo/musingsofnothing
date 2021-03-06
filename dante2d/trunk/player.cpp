#include <iostream>
#include "player.h"
#include "assert.h"
////////////////////////////////
//StateFalling
///////////////////////////////
StateFalling::StateFalling() {
  FALL_ACCEL = -2;
  FALL_SPEED = -5;
  ControlMap &controls = ControlMap::Instance();
  shoot = controls.find_control("SHOOT");
}

bool StateFalling::nextState(PlayerClass* player) {

  if(Collision::map_collision(player) == Collision::COLLISION_BLIMIT) {
    player->currentState = new StateStanding;
    delete this;
    return true;
  } 
  else if (shoot->check_status()) {
    player->currentState = new StateJumpShooting;
    delete this;
    return true;
  } 
  else { //Default no buttons pressed
    //Nothing
    return false;
  }
  //Should never reach here
  assert(true);
}

void StateFalling::setMovement(PlayerClass* player) {
  //Set Movement speeds DON'T 
  player->set_accel(0, FALL_ACCEL);
  player->set_vel(0, FALL_SPEED);
}

void StateFalling::setAnimation(PlayerClass* player) {
  player->currentAnimation = player->dante_jumping;
}

void StateFalling::check_events() {
}
////////////////////////////////
//StateJumpShoot
///////////////////////////////
StateJumpShooting::StateJumpShooting() {
  JUMP_ACCEL = 0;
  JUMP_SPEED = -4;
  ControlMap &controls = ControlMap::Instance();
  shoot = controls.find_control("SHOOT");
}

bool StateJumpShooting::nextState(PlayerClass* player) {

  if(Collision::is_collided(player, 0) == Collision::COLLISION_BLIMIT) {
    player->currentState = new StateStanding;
    delete this;
    return true;
  } 
  else if (shoot->check_status()) {
    //Keep in Shooting state
    return false;
  } 
  else { //Default no buttons pressed
     player->currentState = new StateFalling;
     delete this;
     return true;
  }
  //Should never reach here
  assert(true);
}

void StateJumpShooting::setMovement(PlayerClass* player) {
  //Set XRUN_XPEED positive for run right
  player->set_accel(0, JUMP_ACCEL);
  player->set_vel(0, JUMP_SPEED);
}

void StateJumpShooting::setAnimation(PlayerClass* player) {
  player->currentAnimation = player->dante_jumpshooting;
}

void StateJumpShooting::check_events() {
}
////////////////////////////////
//StateJumping
///////////////////////////////
StateJumping::StateJumping() {
  JUMP_SPEED = 60;
  JUMP_ACCEL = -2;
  JUMP_ACCEL_SMALL = -5;
  ControlMap &controls = ControlMap::Instance();
  jump  = controls.find_control("JUMP");
  shoot  = controls.find_control("SHOOT");
}

bool StateJumping::nextState(PlayerClass* player) {

  //Reduce the jump height if the button isn't still pressed
  if(!jump->check_status() && 
     (jump->active_time.get_elapsed() < 300) ) {
    player->set_accel(0, JUMP_ACCEL_SMALL);
  }
  if(Collision::map_collision(player) == Collision::COLLISION_BLIMIT) {
    player->currentState = new StateStanding;
    delete this;
    return true;
  } 
  else if( shoot->check_status() ) {
    player->currentState = new StateJumpShooting;
    delete this;
    return true;
  } 

  else { //Default no buttons pressed
//     player->currentState = new StateStanding;
//     delete this;
//     return true;
    return false;
  }
  //Should never reach here
  assert(true);
}

void StateJumping::setMovement(PlayerClass* player) {
  //Set XRUN_XPEED positive for run right
  player->set_accel(0, JUMP_ACCEL);
  player->set_vel(0, JUMP_SPEED);
}

void StateJumping::setAnimation(PlayerClass* player) {
  player->currentAnimation = player->dante_jumping;
}

void StateJumping::check_events() {
}

////////////////////////////////
//StateRunRight
///////////////////////////////
StateRunRight::StateRunRight() {
  XRUN_SPEED = 10;
  ControlMap &controls = ControlMap::Instance();
  right = controls.find_control("MV_RIGHT");
  left = controls.find_control("MV_LEFT");
}
bool StateRunRight::nextState(PlayerClass* player) {

  if (right->check_status()) {
     return false;
   } 
  else if(left->check_status()) {
    player->currentState = new StateRunLeft;
    delete this;
    return true;
  } 
  else { //Default no buttons pressed
    player->currentState = new StateStanding;
    delete this;
    return true;
  }
  //Should never reach here
  assert(true);
}

void StateRunRight::setMovement(PlayerClass* player) {
  //Set XRUN_XPEED positive for run right
  player->set_accel(0, 0);
  player->set_vel(XRUN_SPEED, 0);
}

void StateRunRight::setAnimation(PlayerClass* player) {
  player->currentAnimation = player->dante_runright;
}

void StateRunRight::check_events() {
}

////////////////////////////////
//StateRunLeft
///////////////////////////////
StateRunLeft::StateRunLeft() {
  YRUN_SPEED = -10;
  ControlMap &controls = ControlMap::Instance();
  right = controls.find_control("MV_RIGHT");
  left = controls.find_control("MV_LEFT");
}
bool StateRunLeft::nextState(PlayerClass* player) {
  if (left->check_status()) {
    return false;
  } 
  else if(right->check_status()) {
    player->currentState = new StateRunRight;
    delete this;
    return true;
  } 
  else { //Default no buttons pressed
    player->currentState = new StateStanding;
    delete this;
    return true;
  }
  //Should never reach here
  assert(true);
}

void StateRunLeft::setMovement(PlayerClass* player) {
  player->set_accel(0, 0);
  player->set_vel(YRUN_SPEED, 0);
}

void StateRunLeft::setAnimation(PlayerClass* player) {
  player->currentAnimation = player->dante_runleft;
}

void StateRunLeft::check_events() {
}

////////////////////////////////
//StateStanding functions
///////////////////////////////

bool StateStanding::nextState(PlayerClass* player) {
  ControlMap &controls = ControlMap::Instance();

  if (controls.check_status("MV_LEFT")) {
    player->currentState = new StateRunLeft;
    delete this;
    return true;
  } 
  else if(controls.check_status("MV_RIGHT")) {
    player->currentState = new StateRunRight;
    delete this;
    return true;
  }
  else if(controls.check_status("JUMP")) {
    player->currentState = new StateJumping;
    delete this;
    return true;
  } 
  else { //Default no buttons pressed
    return false;
  }
  //Should never reach here
  assert(true);
}

void StateStanding::setMovement(PlayerClass* player) {
  player->set_accel(0 ,0);
  player->set_vel(0, 0);
}

void StateStanding::setAnimation(PlayerClass* player) {
  player->currentAnimation = player->dante_standing;
}

void StateStanding::check_events() {
}

////////////////////////////////
//PlayerClass Functions
///////////////////////////////
PlayerClass::PlayerClass(Sint16 posx, Sint16 posy)
  :cGameObject(posx, posy)
{
  //Usually new states will require a separate animation, but not always
  //Make sure corresponding delete exists
  dante_runright = new animSprite("sprites/dante_runright");
  dante_runleft = new animSprite("sprites/dante_standing");
  dante_standing = new animSprite("sprites/dante_standing");
  dante_jumping = new animSprite("sprites/dante_standing");
  dante_jumpshooting = new animSprite("sprites/dante_shooting");

  //A player starting state and animation
  currentState = new StateStanding;
  currentAnimation = dante_standing;

  //save the world dimensions the player can travel in
  assert(LevelData::current_level != 0);
  LevelData::current_level->get_world_max(world_x_max, world_y_max);
}

PlayerClass::~PlayerClass() {
  //Real potential for memory leaks, make sure these correspond with
  //Cstor new statements, currentAnimation delete not needed
  delete currentState;
  delete dante_runright;
  delete dante_runleft;
  delete dante_standing;
  delete dante_jumping;
  delete dante_jumpshooting;
}
//FIX ME
void PlayerClass::draw() {
  currentAnimation->set_pos(position.x, position.y);
  currentAnimation->draw();
}

void PlayerClass::logic() {
  if(currentState->nextState(this)) {
    currentState->setMovement(this);
    currentState->setAnimation(this);
  }
}

void PlayerClass::read_dims(Sint16 &read_dimx, Sint16 &read_dimy) const {
  currentAnimation->read_dims(read_dimx, read_dimy);
}

void PlayerClass::move() {

   velx += accelx;
   vely += accely;
   oldposition.x = position.x;
   oldposition.y = position.y;

   world_x += velx;
   world_y +=vely;

   if (!LevelData::isScrollx_min) {
     position.x = world_x;
   }
   else if(!LevelData::isScrollx_max){
     //world x / position.x
     position.x = world_x - (world_x_max - Engine::SCREEN_W);
   }

   if (!LevelData::isScrolly_min) {
     position.y = world_y;
   } 
   else if(!LevelData::isScrolly_max){
     //world x / position.x
     position.y = world_y - (world_y_max - Engine::SCREEN_H);
   }

}
