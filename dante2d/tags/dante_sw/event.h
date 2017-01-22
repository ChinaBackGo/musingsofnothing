#ifndef INC_EVENT_H
#define INC_EVENT_H

class cEvent
{

 public:
  static bool s_done; //move this to manager class? FIX ME
  static bool frame_update;
  static void poll_events();
};

#endif  /* INC_EVENT_H */
