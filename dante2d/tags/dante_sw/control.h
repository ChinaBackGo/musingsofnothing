#ifndef INC_CONTROL_H
#define INC_CONTROL_H

#include <vector>
#include <string>
#include "SDL/SDL.h"
#include "engine.h"

class Control {
  bool is_active;
  SDLKey key;
  std::string name;


 public:
  Control(SDLKey new_key, std::string new_name);
  ~Control();

  //can these be one function?
  void set_status(const bool status);
  bool check_status() const;
  SDLKey read_key() const;
  void set_key(const SDLKey new_key);
  std::string read_name() const;
  Timer active_time;
};

//A Meyers Singleton ControlMap
class ControlMap {
 public:
  static ControlMap& Instance() {
    static ControlMap theControlMap;
    return theControlMap;
  }
  /* more (non-static) functions here */
  bool check_status(const std::string &name);
  void set_status(SDLKey key, bool status);
  //  void add_control(SDLKey key);
  //  void change_bind (std::string name);
  Control* find_control(const SDLKey &key);
  Control* find_control(const std::string &name);  
  void reset();
  
 private:
  ControlMap(); // ctor hidden
  //  ControlMap(ControlMap const&); // copy ctor hidden
  //  ControlMap& operator=(ControlMap const&); // assign op. hidden
  ~ControlMap(); // dtor hidden
  static std::vector<Control> control_map_vec;
};

#endif  /* INC_CONTROL_H */
