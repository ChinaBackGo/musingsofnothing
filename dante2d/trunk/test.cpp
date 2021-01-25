#include <iostream>
#include <cstdlib> // For some useful functions such as atexit :)
#include <string>
#include <sstream>
#include <typeinfo>
#include <SDL.h> // main SDL header
#include "event.h"
#include "sprite.h"
#include "control.h"
#include "player.h"
#include "engine.h"
#include "texture.h"
#include "world.h"
#include "collision.h"

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
void test_screen_res();
void test_texture();
void test_Control();
void test_ControlMap();
void test_animSprite();
void test_timer();
void test_StateRunRight();
void test_StateRunLeft();
void test_StateStanding();
void test_cGameObject();
void test_PlayerClass();
void test_Collision();
void test_tile();
void test_tilemap();
void test_leveldata();

int main (int argc, char *argv[])
{
  //Static engine init function
  Engine::init();

  //TODO prints the working director - pass this to engine init? TODO
  std::cout << argv[0];
   
  //Breaks dynamically loaded libs?  FIX ME put this in cleanup code
  atexit(SDL_Quit); //Now that we're enabled, make sure we cleanup

  //Call our Screen singleton  
  Screen &testscreen = Screen::Instance();
  //
  ControlMap &testmap = ControlMap::Instance();
  testmap.reset();
  /****************TEST FUNCTIONS*****************/
   test_screen_res();
   test_texture();
   test_Control();
   test_ControlMap();
   test_animSprite();
   test_timer();
   test_StateRunRight();
   test_StateRunLeft();
   test_StateStanding();
   test_cGameObject();
   test_PlayerClass();
   test_Collision();
   test_tile();
   test_tilemap();
   test_leveldata();


  //  TileMap test_tilemap("maps/testmap.map",0, 0, false);
  //Test scrolling with level data
  LevelData test_level;
  test_level.read_settings("maps/tiled.tmx");
  LevelData::current_level = &test_level;

  PlayerClass testplayer(0, 100);
  testplayer.set_world_pos(0,100);

  Uint32 frame_rate = 0;
  Timer frame_timer;
  Sint32 posx, posy;
  float cam_z, cam_x, cam_y;

  testscreen.read_cam(cam_x, cam_y, cam_z);

  while(!(cEvent::s_done)){ //While program isn't done
    //Event
    cEvent::poll_events();
    //Logic
    testplayer.logic();

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
      
      testplayer.read_world_pos(posx, posy);
      glLoadIdentity();
      glTranslatef(cam_x, cam_y, cam_z /*+ abs(posx)*1.5*/);

      //move  logic
      test_level.Scroller(posx,posy);
      testplayer.move();
      testplayer.logic();
      //  testplayer.print();

      //Clear buffers
      testscreen.clear();

      //Draw
      test_level.draw();
      testplayer.draw();

      //Screen update done
      testscreen.update();

      frame_rate++;
      cEvent::frame_update = false;
    }
  } //Program done, exited
  return 0;
}
void test_screen_res() {
  Sint16 XMAX, YMAX;
  Screen &testscreen = Screen::Instance();
  XMAX = Engine::SCREEN_W;
  YMAX = Engine::SCREEN_H;
  string test_texture_str = R"(D:\Dev\musingsofnothing\dante2d\trunk\dante2d\x64\Debug\sprites\test_square.png)";
  // TODO - fix the diretory where images are located, to be relative to the running director. get this from main.
  Texture testtex(test_texture_str, 0, 0);
  Texture testtex1(test_texture_str, 0, YMAX - 64);
  Texture testtex2(test_texture_str, XMAX - 64, YMAX-64);
  Texture testtex3(test_texture_str, XMAX- 64, 0);
  Texture testtex4(test_texture_str, XMAX/2, YMAX/2);
  testscreen.clear();
  testtex.draw();
  testtex1.draw();
  testtex2.draw();
  testtex3.draw();
  testtex4.draw();
  testscreen.update();
  SDL_Delay(2000);
}
void test_texture() {
  //  engineTest test;
  string test_str = "texture";
  Texture testtex("sprites/testguy.png", 0,0);
  Texture testtex1("sprites/dante_standing00.png", 100,250);
  Texture testtex2("sprites/dante_standing01.png", 200,250);

  Screen &testscreen = Screen::Instance();

  testscreen.clear();

  testtex.draw();
  testtex.set_pos(10,10);
  testtex.draw();

  testtex.set_pos(100,0);
  testtex.draw();
  testtex1.draw();
  testtex2.draw();

  testscreen.update();

  //Pause for static drawing tests
  SDL_Delay(2000);
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

 void test_animSprite() {
   //   engineTest test;
   string test_str = "animSprite";
   animSprite testsprite("sprites/dante_standing", 0, 100);
   Screen &testscreen = Screen::Instance();
   for (int i = 0; i < 100; i++){
     testscreen.clear();
     testsprite.draw();
     testscreen.update();
     SDL_Delay(30);
   }

}

void test_timer() {
  engineTest test;
  Timer testtimer;
  testtimer.start();
  SDL_Delay(1000);
  test.test_value(1000, testtimer.get_elapsed(), "Timer");
}

void test_StateRunRight() {
  //  Screen &testscreen = Screen::Instance();
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
  Sint16 testx, testy;
  test_state = new StateRunRight;
  test_state->setMovement(&theplayer);
  theplayer.read_vel(testx,testy);
  test.test_value(testx, 10, test_str + " setMovement x");
  test.test_value(testy, 0, test_str + " setMovement y");

  //Test animation test_state still is a valid reference
  //see if animation changes
  theplayer.draw();
  test_state->setAnimation(&theplayer);
  theplayer.set_pos(40, 300);
  theplayer.draw();

  delete test_state;
}

void test_StateRunLeft() {
  //  Screen &testscreen = Screen::Instance();
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
  Sint16 testx, testy;
  test_state = new StateRunLeft;
  test_state->setMovement(&theplayer);
  theplayer.read_vel(testx,testy);
  test.test_value(testx, -10, test_str + " setMovement x");
  test.test_value(testy, 0, test_str + " setMovement y");

  //Test animation test_state still is a valid reference
  //see if animation changes
  theplayer.draw();
  test_state->setAnimation(&theplayer);
  theplayer.set_pos(100, 300);
  theplayer.draw();

  delete test_state;
}

void test_StateStanding() {
  //  Screen &testscreen = Screen::Instance();
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
  Sint16 testx, testy;
  test_state = new StateStanding;
  test_state->setMovement(&theplayer);
  theplayer.read_vel(testx,testy);
  test.test_value(testx, 0, test_str + " setMovement x");
  test.test_value(testy, 0, test_str + " setMovement y");

  //Test animation test_state still is a valid reference
  //see if animation changes
  theplayer.draw();
  test_state->setAnimation(&theplayer);
  theplayer.set_pos(250, 300);
  theplayer.draw();

  delete test_state;
}

void test_cGameObject() {
  engineTest test;
  string test_str = "Game Object";
  int posx, posy;
  Sint16 velx, vely, accelx, accely; 
  Sint16 t_velx, t_vely, t_accelx, t_accely;
  Sint16 t_dimx, t_dimy;
  Sint16 t_posx, t_posy;

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

  Sint16 dimx, dimy;
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
  //  Screen &testscreen = Screen::Instance();
  engineTest test;
  string test_str = "PlayerClass";

  //Test drawing
  PlayerClass theplayer(0,200);
  theplayer.draw();
  theplayer.set_pos(40,200);
  theplayer.draw();
  theplayer.set_pos(80,200);
  theplayer.draw();
  theplayer.set_pos(120,200);
  theplayer.draw();

  //Test the get dims
  Sint16 dimx, dimy;
  //Standing animation, first frame, width/height from dante_standing.settings 52 79
  theplayer.read_dims(dimx, dimy);
  test.test_value(dimx, 128, test_str + " dimx");
  test.test_value(dimy, 128, test_str + " dimy");

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
  theplayer.set_pos(0, Engine::SCREEN_H * -1 -10);
  
  theplayer.logic();
  test.test_value(typeid(*(theplayer.currentState)) == typeid(StateStanding), true,
		  test_str + " Jumping -> Standing transition");
  
  //Standing Tests Complete
}

void test_Collision () {
  engineTest test;
  string test_str = "Collision";
  LevelData test_level;
  test_level.read_settings("maps/tiled.tmx");
  Sint32 posx, posy;
  LevelData::current_level = &test_level;

  //Lets be honest...these are shit tests
  PlayerClass testplayer(0,0);
  animSprite testsprite("sprites/dante_runright", 0, 0);
  //test if there is overlap
  test.test_value(Collision::is_collided(&testplayer, &testsprite), 
		  Collision::COLLISION_TRUE, test_str);

  testsprite.set_pos(129,100);
  //set the sprite out of overlap and test
  test.test_value(Collision::is_collided(&testplayer, &testsprite), 
		  Collision::COLLISION_NULL, test_str);

  cGameObject testobjectA(0,0,10,10);
  cGameObject testobjectB(0,0,40,60);

  //Time for the real tests!
  //A
  //B
  //No intersection

  testobjectA.set_pos(110,69);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_NULL, test_str);

  //AB
  //No intersection
  testobjectA.set_pos(89,90);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_NULL, test_str);

  //B
  //A
  //No intersection
  testobjectA.set_pos(110,141);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_NULL, test_str);

  //BA
  //No intersection
  testobjectA.set_pos(141,90);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_NULL, test_str);

  //INTERSECTION tests
  //A
  //B

  testobjectA.set_pos(110,70);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_TRUE, test_str);

  //AB
  //Intersecting
  testobjectA.set_pos(90,90);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_TRUE, test_str);

  //B
  //A
  //Intersecting
  testobjectA.set_pos(110,140);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_TRUE, test_str);

  //BA
  //Intersecting
  testobjectA.set_pos(140,90);
  testobjectB.set_pos(100,80);

  test.test_value(Collision::is_collided(&testobjectA, &testobjectB), 
		  Collision::COLLISION_TRUE, test_str);
  
  //Test Player if it falls off screen
  test.test_value(Collision::is_collided(&testplayer, 0), 
		  Collision::COLLISION_NULL, test_str);

  testplayer.set_pos(0,Engine::SCREEN_H * -1 - 10);
  test.test_value(Collision::is_collided(&testplayer, 0), 
		  Collision::COLLISION_BLIMIT, test_str);


  //Test obj_collision function
  //A 10x10, B 40x60
  testobjectA.set_world_pos(0,70);
  testobjectB.set_world_pos(0,0);

  test.test_value(Collision::obj_collision(&testobjectA, &testobjectB), 
		  Collision::COLLISION_NULL, test_str + " obj_collision");

  //Test obj_collision function
  //A 10x10, B 40x60
  testobjectA.set_world_pos(0,40);
  testobjectB.set_world_pos(0,0);

  test.test_value(Collision::obj_collision(&testobjectA, &testobjectB), 
		  Collision::COLLISION_BLIMIT, test_str + " obj_collision");



  //Test collision Map
  

  testobjectA.set_world_pos(0, 33);


  test.test_value(Collision::map_collision(&testobjectA),
		  Collision::COLLISION_BLIMIT, test_str + " map collision 1");
  testobjectA.read_world_pos(posx, posy);
  test.test_value(posy, 64, test_str + " objA overshoot");

  testobjectA.set_world_pos(0, 100);
  test.test_value(Collision::map_collision(&testobjectA),
		  Collision::COLLISION_NULL, test_str + " map collision 2");
}

void test_tile() {
  //  Screen &testscreen = Screen::Instance();
  //  engineTest test;
  string test_str = "Tile";

  Tile test_tile("maps/tiles/slate.png", false);
  Tile test_anim("sprites/dante_runright", true);

}

void test_tilemap() {
  // Screen &testscreen = Screen::Instance();
  //  engineTest test;
  //  string test_str = "TileMap";

  //  TileMap test_tilemap("maps/testmap.map",0, 0, false);
  //  test_tilemap.print_map();

//   testscreen.clear();
//   test_tilemap.set_worldpos(10,10);
//   test_tilemap.draw();
//   testscreen.update();
//   SDL_Delay(2000);
}

void test_leveldata() {
  Screen &testscreen = Screen::Instance();
  engineTest test;
  string test_str = "LevelData";
  LevelData test_level;
  Sint32 result;

  test.test_value(test_level.read_settings("maps/tiled.tmx"), true, test_str);
  string test_str2 = "<map version=\"0.99b\" orientation=\"orthogonal\" width=\"5\"height=\"5\" tilewidth=\"32\" tileheight=\"32\">";
  result = test_level.get_map_data_int(test_str2, "width");
  test.test_value(result, 5, "LevelData width");

  result = test_level.get_map_data_int(test_str2, "height");
  test.test_value(result, 5, "LevelData height");

  result = test_level.get_map_data_int(test_str2, "tilewidth");
  test.test_value(result, 32, "LevelData tilewidth");

  result = test_level.get_map_data_int(test_str2, "tileheight");
  test.test_value(result, 32, "LevelData tileheight");

  string test_str3 = " <tileset firstgid=\"1\" source=\"tiled_tileset1.tsx\"/>";
  string result_str;
  result_str = test_level.get_map_data_str(test_str3, "source");
  test.test_value(result_str, "tiled_tileset1.tsx", test_str + " tiledef");

  test_level.print();
  testscreen.clear();
  test_level.Scroller(0,0);
  test_level.draw();
  testscreen.update();
  SDL_Delay(1000);

  testscreen.clear();
  test_level.Scroller(10,0);
  test_level.draw();
  testscreen.update();
  SDL_Delay(1000);

  testscreen.clear();
  test_level.Scroller(1000,0);
  test_level.draw();
  testscreen.update();
  SDL_Delay(1000);

  testscreen.clear();
  test_level.Scroller(0,700);
  test_level.draw();
  testscreen.update();
  SDL_Delay(1000);

}
