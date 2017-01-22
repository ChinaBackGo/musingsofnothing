#include <iostream>
#include <cstdlib> // For some useful functions such as atexit :)
#include <string>
#include <sstream>
#include <typeinfo>
#include "SDL/SDL.h" // main SDL header
#include "event.h"

#include "sprite.h"
#include "control.h"
#include "player.h"
#include "engine.h"
using namespace std;

class engineTest {
  static Uint32 testcount;
public:
  void passed(string &test_str) {
    testcount++;
    cout << "Test # " << testcount << " : Passed!" << " " << test_str << endl;
  }
  void failed(string &test_str, int expected = 0, int test = 0) {
    testcount++;
    stringstream fail_str;
    fail_str << "Test # " << testcount << " : !!FAILED!!" << " " << test_str;

    if (expected == test)
      cout << fail_str.str() << endl;
    else 
      cout << fail_str.str()  << " Expected: " << expected << " Test: " << test << endl;
  }

  void test_value (int test, int expected, string test_str)
  {

    if (expected  == test) {
      passed(test_str);
    } else {
      failed(test_str, expected, test);
    }
  }
  
  void test_value (bool test, bool expected, string test_str)
  {
    if (expected  == test) {
      passed(test_str);
    } else {
      failed(test_str);
    }
  }
  
  void test_value (string test, string expected, string test_str)
  {
    if (expected  == test) {
      passed(test_str);
    } else {
      failed(test_str);
    }
  }
};
Uint32 engineTest::testcount = 0;

//Test Functions
void test_cSprite(SDL_Surface *dest);
void test_Control();
void test_ControlMap();
void test_animSprite(SDL_Surface *dest);
void test_timer();
void test_StateRunRight();
void test_StateRunLeft();
void test_StateStanding();
void test_cGameObject();
void test_PlayerClass();
void test_Collision();

int main (int argc, char *argv[])
{
  //Static engine init function
  Engine::init();
  
  //Breaks dynamically loaded libs?  FIX ME put this in cleanup code
  atexit(SDL_Quit); //Now that we're enabled, make sure we cleanup

  //Call our Screen singleton  
  Screen &testscreen = Screen::Instance();

  /****************TEST FUNCTIONS*****************/
  test_Control();
  test_cSprite(testscreen.get());
  test_ControlMap();
  test_animSprite(testscreen.get());
  test_timer();
  test_StateRunRight();
  test_StateRunLeft();
  test_StateStanding();
  test_cGameObject();
  test_PlayerClass();
  test_Collision();

  testscreen.update();
  //Pause for static drawing tests
  SDL_Delay(2000);

  animSprite testsprite1("sprites/dante_runright", 0, 0);
  animSprite testsprite2("sprites/dante_runleft", 100, 0);
  animSprite testsprite3("sprites/dante_standing", 200, 0);

  PlayerClass testplayer(500, 470);

  Uint32 frame_rate = 0;
  Timer frame_timer;

  while(!(cEvent::s_done)){ //While program isn't done
    //Event
    cEvent::poll_events();
    //Logic
    //    testplayer.logic();

    //Basic frame rate calculator, should be refactored into Engine?
    if (frame_timer.get_elapsed() > 1000) {
      stringstream caption;
      caption << "Frame Rate: " << frame_rate << endl;
      SDL_WM_SetCaption( caption.str().c_str(), NULL );
      frame_timer.start();
      frame_rate = 0;
    }
    //Draw
    if (cEvent::frame_update) {
    
      testscreen.update();
      testscreen.fill_white();

      //move  logic
      testplayer.move();
      testplayer.logic();


      //Draw
      testsprite1.draw(testscreen.get());
      testsprite2.draw(testscreen.get());
      testsprite3.draw(testscreen.get());
      testplayer.draw(testscreen.get());
      //Screen update done
      frame_rate++;
      cEvent::frame_update = false;
    }
  } //Program done, exited
  return 0;
}

void test_cSprite(SDL_Surface *screen) {
  engineTest test;
  string test_str = "Csprite";

  Sint32 x, y;
  //testing draw
  cSprite test_sprite2("sprites/dante_runright", 100, 0);

  test_sprite2.draw(screen);
  
  //testing move
  test_sprite2.set_vel(80, 0);
  test_sprite2.move();
  test_sprite2.read_vel(x,y);
  //test_sprite2.draw(screen);
  test.test_value(80, x, test_str);
  test.test_value(0, y, test_str);
  test_sprite2.set_vel(-10,0);
  test_sprite2.move();
  test_sprite2.draw(screen);
  test_sprite2.move();
  test_sprite2.draw(screen);
  Uint32 dimx, dimy;

  test_sprite2.read_dims(dimx, dimy);
  test.test_value(dimx, 419, test_str + " dim x test");
  test.test_value(dimy, 83, test_str + " dim y test");
}

void test_Control()
{
  engineTest test;
  string test_str = "Control";

  Control test_control(SDLK_LEFT, "move_left");
  test.test_value(test_control.check_status(), false, test_str);
  test_control.set_status(true);
  test.test_value(test_control.check_status(), true, test_str);
  test_control.set_status(false);
  test.test_value(test_control.check_status(), false, test_str);
  
  if (test_control.read_key() == SDLK_LEFT)
    test.passed(test_str);
  else
    test.failed(test_str);

  test_control.set_key(SDLK_UP);
  
  if (test_control.read_key() == SDLK_UP)
    test.passed(test_str);
  else
    test.failed(test_str);

  test.test_value(test_control.read_name(), "move_left", test_str);
  
  test_control.set_status(true);
  SDL_Delay(500);
  test.test_value(test_control.active_time.get_elapsed() > 490, true, 
		  test_str + " control timer");
  SDL_Delay(500);
  test.test_value(test_control.active_time.get_elapsed() < 1100, true, 
		  test_str + " control timer");
}

void test_ControlMap()
{
  engineTest test;
  string test_str = "Control Map";
  ControlMap &testmap = ControlMap::Instance();
  test.test_value(testmap.check_status("MV_LEFT"), false, test_str);
  testmap.set_status(SDLK_z, true); //Melee
  testmap.set_status(SDLK_x, true);//shoot
  testmap.set_status(SDLK_RIGHT, true);
  testmap.set_status(SDLK_DOWN, true);
  test.test_value(testmap.check_status("SHOOT"), true, test_str);
  test.test_value(testmap.check_status("MELEE"), true, test_str);
  test.test_value(testmap.check_status("JUMP"), false, test_str);

  testmap.set_status(SDLK_RIGHT, false);
  testmap.set_status(SDLK_DOWN, false);

  test.test_value(testmap.check_status("MV_RIGHT"), false, test_str);
  test.test_value(testmap.check_status("MV_DOWN"), false, test_str);

  ControlMap& testmap2 = testmap;
  ControlMap& testmap3 = ControlMap::Instance();

  test.test_value(testmap2.check_status("SHOOT"), true, test_str);
  test.test_value(testmap3.check_status("MELEE"), true, test_str);
  test.test_value(testmap2.check_status("JUMP"), false, test_str);
  test.test_value(testmap3.check_status("MV_RIGHT"), false, test_str);
  test.test_value(testmap2.check_status("MV_DOWN"), false, test_str);

  //Test map reset
  testmap.set_status(SDLK_z, true); //Melee
  testmap.set_status(SDLK_x, true);//shoot
  testmap.set_status(SDLK_RIGHT, true);//move right

  testmap.reset();
  
  test.test_value(testmap2.check_status("SHOOT"), false, test_str + " reset");
  test.test_value(testmap3.check_status("MELEE"), false, test_str + " reset");
  test.test_value(testmap2.check_status("JUMP"), false, test_str + " reset");
  test.test_value(testmap3.check_status("MV_RIGHT"), false, test_str + " reset");
  test.test_value(testmap2.check_status("MV_DOWN"), false, test_str + " reset");
  
}
void test_animSprite(SDL_Surface *dest) {
  engineTest test;
  string test_str = "animSprite";
  animSprite testsprite("sprites/dante_runright", 0, 100);
  testsprite.draw(dest);
  testsprite.reset_frame();
  SDL_Rect testrect;
  testrect.x = 20;
  testrect.y = 100;

  //Test Overloaded draw function
  testsprite.draw(dest, testrect);

  //From dante_runright.settings first clip is 74 wide, and 83 height
  testsprite.reset_frame();


  Uint32 dimx, dimy;
  testsprite.read_dims(dimx, dimy);
  test.test_value(dimx, 74, test_str + " dimx");
  test.test_value(dimy, 83, test_str + " dimy");  
}

void test_timer() {
  engineTest test;
  Timer testtimer;
  testtimer.start();
  SDL_Delay(1000);
  test.test_value(1000, testtimer.get_elapsed(), "Timer");
}

void test_StateRunRight() {
  Screen &testscreen = Screen::Instance();
  engineTest test;
  string test_str = "StateRunRight";
  AbstractStateClass *test_state = new StateRunRight;
  PlayerClass theplayer(0, 300);
  ControlMap& testmap = ControlMap::Instance();
  testmap.set_status(SDLK_RIGHT, true);

  test.test_value (test_state->nextState(&theplayer), false, test_str);

  //Could delete test_state here, but we use it again.  

  testmap.set_status(SDLK_RIGHT, false);
  testmap.set_status(SDLK_LEFT, true);

  //Delete player status' current state of Standing   
  delete theplayer.currentState;

  test.test_value (test_state->nextState(&theplayer), true, test_str);
  //Test value has been deleted
  //theplayer.currentState is now a new StateRunLeft which is deleted by Players Dstor

  //reset our controls! its a Singleton ;)
  testmap.set_status(SDLK_LEFT, false);

  //Continue to test, test_state has been deleted by state change, need a new reference
  int testx, testy;
  test_state = new StateRunRight;
  test_state->setMovement(&theplayer);
  theplayer.read_vel(testx,testy);
  test.test_value(testx, 10, test_str + " setMovement x");
  test.test_value(testy, 0, test_str + " setMovement y");

  //Test animation test_state still is a valid reference
  //see if animation changes
  theplayer.draw(testscreen.get());
  test_state->setAnimation(&theplayer);
  theplayer.set_pos(40, 300);
  theplayer.draw(testscreen.get());

  delete test_state;
}

void test_StateRunLeft() {
  Screen &testscreen = Screen::Instance();
  engineTest test;
  string test_str = "StateRunRight";
  AbstractStateClass *test_state = new StateRunLeft;
  PlayerClass theplayer(100, 300);
  ControlMap& testmap = ControlMap::Instance();
  testmap.set_status(SDLK_LEFT, true);

  test.test_value (test_state->nextState(&theplayer), false, test_str);

  //Could delete test_state here, but we use it again.  

  testmap.set_status(SDLK_RIGHT, true);
  testmap.set_status(SDLK_LEFT, false);

  //Delete player status' current state of Standing   
  delete theplayer.currentState;

  test.test_value (test_state->nextState(&theplayer), true, test_str);
  //Test value has been deleted
  //theplayer.currentState is now a new StateRunRight which is deleted by Players Dstor

  //reset our controls! its a Singleton ;)
  testmap.set_status(SDLK_RIGHT, false);

  //Continue to test, test_state has been deleted by state change, need a new reference
  int testx, testy;
  test_state = new StateRunLeft;
  test_state->setMovement(&theplayer);
  theplayer.read_vel(testx,testy);
  test.test_value(testx, -10, test_str + " setMovement x");
  test.test_value(testy, 0, test_str + " setMovement y");

  //Test animation test_state still is a valid reference
  //see if animation changes
  theplayer.draw(testscreen.get());
  test_state->setAnimation(&theplayer);
  theplayer.set_pos(100, 300);
  theplayer.draw(testscreen.get());

  delete test_state;
}

void test_StateStanding() {
  Screen &testscreen = Screen::Instance();
  engineTest test;
  string test_str = "StateStanding";
  AbstractStateClass *test_state = new StateStanding;
  PlayerClass theplayer(200, 300);
  ControlMap& testmap = ControlMap::Instance();

  test.test_value (test_state->nextState(&theplayer), false, test_str);

  //Could delete test_state here, but we use it again.  

  testmap.set_status(SDLK_RIGHT, true);
  testmap.set_status(SDLK_LEFT, false);

  //Delete player status' current state of Standing   
  delete theplayer.currentState;

  test.test_value (test_state->nextState(&theplayer), true, test_str);
  //Test value has been deleted
  //theplayer.currentState is now a new StateRunRight which is deleted by Players Dstor

  //reset our controls! its a Singleton ;)
  testmap.set_status(SDLK_RIGHT, false);

  //Continue to test, test_state has been deleted by state change, need a new reference
  int testx, testy;
  test_state = new StateStanding;
  test_state->setMovement(&theplayer);
  theplayer.read_vel(testx,testy);
  test.test_value(testx, 0, test_str + " setMovement x");
  test.test_value(testy, 0, test_str + " setMovement y");

  //Test animation test_state still is a valid reference
  //see if animation changes
  theplayer.draw(testscreen.get());
  test_state->setAnimation(&theplayer);
  theplayer.set_pos(250, 300);
  theplayer.draw(testscreen.get());

  delete test_state;
}

void test_cGameObject() {
  engineTest test;
  string test_str = "Game Object";
  int posx, posy;
  Sint32 velx, vely, accelx, accely; 
  Sint32 t_velx, t_vely, t_accelx, t_accely;
  Uint32 t_dimx, t_dimy;
  Uint32 t_posx, t_posy;

  t_posx = t_posy = 0;
  posx = 32;
  posy = 344;
  velx = 1113;
  vely = 3993;
  t_dimx = 33;
  t_dimy = 49;

  cGameObject test_object(posx, posy, t_dimx, t_dimy);
  
  test_object.read_pos(t_posx, t_posy);
  test.test_value(posx, t_posx, test_str);
  test.test_value(posy, t_posy, test_str);

  Uint32  pos2x = 45;
  Uint32  pos2y = 983;

  test_object.set_pos(pos2x, pos2y);
  test_object.read_pos(t_posx, t_posy);

  //bad FIX ME, Uint32 or int globally?
  test.test_value((int)pos2x, t_posx, test_str);
  test.test_value((int)pos2y, t_posy,  test_str);


  test_object.set_vel(velx, vely);
  test_object.read_vel(t_velx, t_vely );

  test.test_value((int)velx, t_velx, test_str);
  test.test_value((int)vely, t_vely, test_str);

  Uint32 dimx, dimy;
  test_object.read_dims(dimx, dimy);
  test.test_value((int)dimx, t_dimx, test_str);
  test.test_value((int)dimy, t_dimy, test_str);

  //Test acceleration set/read functions
  test_object.read_accel(t_accelx, t_accely);
  test.test_value(t_accelx, 0, test_str + " acceleration x");
  test.test_value(t_accely, 0, test_str + " acceleration y");
  
  accelx = -1120;
  accely = 1993;

  test_object.set_accel(accelx, accely);
  test_object.read_accel(t_accelx, t_accely);
  test.test_value(t_accelx, accelx, test_str + " acceleration x");
  test.test_value(t_accely, accely, test_str + " acceleration y");

  //Test the move functions
  posx = posy = 100;
  velx = vely = 1;
  accelx = 2;
  accely = -2;

  test_object.set_pos(posx, posy);
  test_object.set_vel(velx,vely);
  test_object.set_accel(accelx,accely);
  
  //Test just hardcode these
  //First move
  test_object.move();
  
  //accel = 2, -2 (stays constant)
  //Vel = 3, -1
  //Pos = 103,99
  test_object.read_accel(t_accelx, t_accely);
  test_object.read_vel(t_velx, t_vely);
  test_object.read_pos(t_posx, t_posy);

  test.test_value(t_accelx, accelx, test_str + " move accelx");
  test.test_value(t_accely, accely, test_str + " move accely");
  test.test_value(t_velx, 3, test_str + " move velx");
  test.test_value(t_vely, -1, test_str + " move vely");
  test.test_value(t_posx, 103, test_str + " move posx");
  test.test_value(t_posy, 99, test_str + " move posy");

  test_object.move();
  test_object.move();

  //accel same
  //Vel = 7, -5
  //Pos = 115,91
  test_object.read_accel(t_accelx, t_accely);
  test_object.read_vel(t_velx, t_vely);
  test_object.read_pos(t_posx, t_posy);

  test.test_value(t_accelx, accelx, test_str + " move accelx");
  test.test_value(t_accely, accely, test_str + " move accely");
  test.test_value(t_velx, 7, test_str + " move velx");
  test.test_value(t_vely, -5, test_str + " move vely");
  test.test_value(t_posx, 115, test_str + " move posx");
  test.test_value(t_posy, 91, test_str + " move posy");
}

void test_PlayerClass () {
  Screen &testscreen = Screen::Instance();
  engineTest test;
  string test_str = "PlayerClass";

  //Test drawing
  PlayerClass theplayer(0,200);
  theplayer.draw(testscreen.get());
  theplayer.set_pos(40,200);
  theplayer.draw(testscreen.get());
  theplayer.set_pos(80,200);
  theplayer.draw(testscreen.get());
  theplayer.set_pos(120,200);
  theplayer.draw(testscreen.get());

  //Test the get dims
  Uint32 dimx, dimy;
  //Standing animation, first frame, width/height from dante_standing.settings 52 79
  theplayer.read_dims(dimx, dimy);
  test.test_value(dimx, 52, test_str + " dimx");
  test.test_value(dimy, 79, test_str + " dimy");

  ControlMap &thecontrolmap = ControlMap::Instance();
  //Test Player state transitions
  //Player is State Standing

  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateStanding), true,
		  test_str + " Standing transition");
  //Standing -> RunLeft
  thecontrolmap.set_status(SDLK_LEFT, true);
  
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateRunLeft), true,
		  test_str + " Standing -> RunLeft transition");
  thecontrolmap.reset();

  //RunLeft -> Standing
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateStanding), true,
		  test_str + " RunLeft -> Standing transition");
  thecontrolmap.reset();
  
  //Standing -> RunRight
  //This will break if the controls are changed...but we should always be using
  //defaults here anyway
  thecontrolmap.set_status(SDLK_RIGHT, true);
  
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateRunRight), true,
		  test_str + " Standing -> RunRight transition");
  thecontrolmap.reset();

  //RunRight -> Standing
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateStanding), true,
		  test_str + " RunRight -> Standing transition");
  thecontrolmap.reset();

  //Standing -> Jump
  Control *jump_ctrl = thecontrolmap.find_control("JUMP");
  jump_ctrl->set_status(true);
  
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateJumping), true,
		  test_str + " Standing -> Jumping transition");
  thecontrolmap.reset();

  //Jump -> Standing
  theplayer.set_pos(0, Engine::SCREEN_H);
  
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateStanding), true,
		  test_str + " Jumping -> Standing transition");
  
  //Standing Tests Complete
}

void test_Collision () {
  engineTest test;
  string test_str = "Collision";
  //Lets be honest...these are shit tests
  PlayerClass testplayer(0,0);
  animSprite testsprite("sprites/dante_runright", 0, 0);
  //test if there is overlap
  test.test_value(Engine::is_collided(&testplayer, &testsprite), 
		  Engine::COLLISION_TRUE, test_str);

  testsprite.set_pos(100,100);
  //set the sprite out of overlap and test
  test.test_value(Engine::is_collided(&testplayer, &testsprite), 
		  Engine::COLLISION_NULL, test_str);

  cGameObject testobjectA(0,0,10,10);
  cGameObject testobjectB(0,0,40,60);

  //Time for the real tests!
  //A
  //B
  //No intersection

  testobjectA.set_pos(110,69);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_NULL, test_str);

  //AB
  //No intersection
  testobjectA.set_pos(89,90);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_NULL, test_str);

  //B
  //A
  //No intersection
  testobjectA.set_pos(110,141);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_NULL, test_str);

  //BA
  //No intersection
  testobjectA.set_pos(141,90);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_NULL, test_str);

  //INTERSECTION tests
  //A
  //B

  testobjectA.set_pos(110,70);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_TRUE, test_str);

  //AB
  //Intersecting
  testobjectA.set_pos(90,90);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_TRUE, test_str);

  //B
  //A
  //Intersecting
  testobjectA.set_pos(110,140);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_TRUE, test_str);

  //BA
  //Intersecting
  testobjectA.set_pos(140,90);
  testobjectB.set_pos(100,80);

  test.test_value(Engine::is_collided(&testobjectA, &testobjectB), 
		  Engine::COLLISION_TRUE, test_str);
  
  //Test Player if it falls off screen
    test.test_value(Engine::is_collided(&testplayer, 0), 
		  Engine::COLLISION_NULL, test_str);

    testplayer.set_pos(0,Engine::SCREEN_H);
    test.test_value(Engine::is_collided(&testplayer, 0), 
		  Engine::COLLISION_BLIMIT, test_str);

}
