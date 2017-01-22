#include "control.h"
#include <iostream>
#include "assert.h"

using std::string;
using std::vector;

vector<Control> ControlMap::control_map_vec;

//control action functions
Control::Control(SDLKey new_key, string new_name)
  :is_active(false), key(new_key), name (new_name) {}

Control::~Control() {}

void Control::set_status(const bool status) 
{
  //if is_active is being set to true, start the timer
  if (is_active = status) {
    active_time.start();
  }
}

bool Control::check_status() const 
{
  return is_active;
}
SDLKey Control::read_key() const 
{
  return key;
}
void Control::set_key(SDLKey new_key)
{
  key = new_key;
}
string Control::read_name() const
{
  return name;
}

//ControlMap functions

ControlMap::ControlMap() {
  //Hard coded default controls
  Control mv_left (SDLK_LEFT,  "MV_LEFT");
  Control mv_right(SDLK_RIGHT, "MV_RIGHT");
  Control mv_up   (SDLK_UP,    "MV_UP");
  Control mv_down (SDLK_DOWN,  "MV_DOWN");
  Control melee   (SDLK_z,     "MELEE");
  Control shoot   (SDLK_x,     "SHOOT");
  Control special (SDLK_s,     "SPECIAL");
  Control jump    (SDLK_c,     "JUMP");

  //Add defaults to control vector
  control_map_vec.push_back(mv_left);
  control_map_vec.push_back(mv_right);
  control_map_vec.push_back(mv_up);
  control_map_vec.push_back(mv_down);
  control_map_vec.push_back(melee);
  control_map_vec.push_back(shoot);
  control_map_vec.push_back(special);
  control_map_vec.push_back(jump);
}
//ControlMap::ControlMap(ControlMap const&) {}
//ControlMap::ControlMap& operator=(ControlMap const&) {}
ControlMap::~ControlMap() {}

//Checks control status
bool ControlMap::check_status(const string &name) {
  Control* temp = find_control(name);
  if (temp != 0) {
    return temp->check_status();
  } else {
    //throw exception! FIX ME
    assert(false);
    return false;
  }

}
//Sets control status by key.sm, for ease of calling from event polling function
void ControlMap::set_status(SDLKey key, bool status) {
  Control* temp = find_control(key);
  if (temp != 0) {
    temp->set_status(status);
  } else {
    std::cout << "Control not found\n";
    //    throw exception! FIX ME
  }
}


Control* ControlMap::find_control(const SDLKey &key) {
  //iterate vec control, return control
  vector<Control>::iterator iter;
  for (iter = control_map_vec.begin(); iter != control_map_vec.end(); ++iter) {
    if ((*iter).read_key() == key)
      return &(*iter);
  }
  return 0;
}

Control* ControlMap::find_control(const string &name) {
  //iterate vec control, return control
  vector<Control>::iterator iter;
  for (iter = control_map_vec.begin(); iter != control_map_vec.end(); ++iter) {
    if ((*iter).read_name() == name)
      return &(*iter);
  }
  assert(false);//Control not found!
  return 0;
}
//Sets all control status to false
void ControlMap::reset() {
  vector<Control>::iterator iter;
  for (iter = control_map_vec.begin(); iter != control_map_vec.end(); ++iter) {
    (*iter).set_status(false);
  }
}
