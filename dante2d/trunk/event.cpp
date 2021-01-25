#include <SDL.h>
#include "event.h"
#include "control.h"

//Safe?
bool cEvent::s_done = false;
bool cEvent::frame_update = false;

void cEvent::poll_events()
{
  SDL_Event event;
  ControlMap &thecontrol = ControlMap::Instance();

  while(SDL_PollEvent(&event)){ //Poll events
    switch(event.type){
 //Check event type
    case SDL_QUIT: //User hit the X (or equivelent)
      cEvent::s_done = true; //Make the loop end
      break; //We handled the event
      //    case SDL_VIDEORESIZE: //User resized window
      //       screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32,
      // 				SDL_HWSURFACE | SDL_RESIZABLE); // Create new window
      //       break; //Event handled, fetch next :)
//     case SDL_MOUSEMOTION: //Mouse motion
//       printf("We got a motion event.\n");
//       printf("Current mouse position is: (%d, %d)\n", event.motion.x, event.motion.y);
//      break;
    case SDL_KEYDOWN: //Key pressed
      thecontrol.set_status(event.key.keysym.sym, true);
      break;
    case SDL_KEYUP: //Key pressed
      thecontrol.set_status(event.key.keysym.sym, false);
      break;
    case SDL_USEREVENT:
      cEvent::frame_update = true;
    break;
    } //Finished with current event
  } //Done with all events for now
}
