#include "collision.h"

using namespace std;

Uint32 Collision::is_collided(cGameObject *objectA, cGameObject *objectB) {
  Sint16 AX1, BX1, AY1, BY1, AX2, BX2, AY2, BY2;
  //Set our bounding boxes for the objectA
  objectA->read_pos(AX1, AY1);
  objectA->read_dims(AX2, AY2);
  assert (AX2 > 0);
  assert (AY2 > 0);

  //Taken from bottom right of cGameObject
  AX2 += AX1;
  AY2 += AY1;
 
  //TEMP Lower limit - FIX ME should be world lower limit or set by world geometry
  //This whole function is bunk, and will be replaced by OOP collision system
  Sint16 LOWER_LIMIT = 0;

  if ( objectB == 0 && (typeid(*objectA) == typeid(PlayerClass)) ) {
    //Fix this, totally broken with new coordinates
    if ( AY1 < LOWER_LIMIT) {
      //find Y over shoot
      Sint16 playx, playy;
      Sint32 playx_w, playy_w;

      objectA->read_pos(playx, playy);
      objectA->read_world_pos(playx_w, playy_w);
      Sint16 yovershoot = abs(AY1 - LOWER_LIMIT);
      playy += yovershoot;
      playy_w += yovershoot;
      
      std::cout << "overshoot" << yovershoot << std::endl;
      objectA->set_pos(playx, playy);
      objectA->set_world_pos(playx_w, playy_w);
      cout << "player pos" << playx_w << " " << playy_w << endl;
      return COLLISION_BLIMIT;
    } else
      return COLLISION_NULL;
  }

  //Set our bounding boxes for objectB
  objectB->read_pos(BX1, BY1);
  objectB->read_dims(BX2, BY2);
  assert (BX2 > 0);
  assert (BY2 > 0);

  BX2 += BX1;
  BY2 += BY1;

  //Test horizontally first
  if (AX2 < BX1)
    return COLLISION_NULL;

  if (AX1 > BX2)
    return COLLISION_NULL;

  if (AY1 > BY2)
    return COLLISION_NULL;

  if (AY2 < BY1)
    return COLLISION_NULL;

  return COLLISION_TRUE;

  //Implement TypeID checking for player/animsprite and pixel perfect collision
  
}

Uint32 Collision::obj_collision(cGameObject *objectA, cGameObject *objectB) {
  Sint32 AX1, BX1, AY1, BY1;
  Sint16 AX2, BX2, AY2, BY2;
  //Set our bounding boxes for the objectA
  objectA->read_world_pos(AX1, AY1);
  objectA->read_dims(AX2, AY2);
  assert (AX2 > 0);
  assert (AY2 > 0);

  //Taken from bottom left of cGameObject
  AX2 += AX1;
  AY2 += AY1;
 
  //Set our bounding boxes for objectB
  objectB->read_world_pos(BX1, BY1);
  objectB->read_dims(BX2, BY2);
  assert (BX2 > 0);
  assert (BY2 > 0);

  BX2 += BX1;
  BY2 += BY1;

  //Test horizontally first
  if (AX2 < BX1)
    return COLLISION_NULL;

  if (AX1 > BX2)
    return COLLISION_NULL;

  if (AY1 > BY2)
    return COLLISION_NULL;

  if (AY2 < BY1)
    return COLLISION_NULL;

  //Collision is Bottom
  if (AY1 < BY2) {
    //find Y over shoot
    Sint16 playx, playy;
    Sint32 playx_w, playy_w;
    Sint16 yovershoot = abs(AY1 - BY2);

    //Only move screen position if we aren't scrolling
    if(!LevelData::isScrolly_min || !LevelData::isScrolly_max) {
      objectA->read_pos(playx, playy);
      playy += yovershoot;
      objectA->set_pos(playx, playy);
    }

    objectA->read_world_pos(playx_w, playy_w);
    playy_w += yovershoot;
    objectA->set_world_pos(playx_w, playy_w);
    
    std::cout << "overshoot map collision" << yovershoot << std::endl;

    return COLLISION_BLIMIT;
  }

  return COLLISION_TRUE;

  //Implement TypeID checking for player/animsprite and pixel perfect collision
  
}

Uint32 Collision::map_collision (cGameObject *objectA) {
  Uint32 COLLISION_STATUS = COLLISION_NULL;

  Sint32 world_x, world_y;
  Sint32 tilesize_x, tilesize_y;
  Sint32 blockdim_y, blockdim_x;
  Sint16 obj_dims_x, obj_dims_y;
  std::vector<cGameObject> map_section;
  
  assert(LevelData::current_level != 0);
  TileMap* collision = LevelData::current_level->get_collision_map();
  
  //Retrieve vars from collision map
  assert(collision != 0);
  //  collision->debug();
  collision->get_tilesize(tilesize_x, tilesize_y);
  assert(tilesize_x > 0);
  assert(tilesize_y > 0);
  
   //Retrieve our vars from objectA
  objectA->read_world_pos(world_x, world_y);
  objectA->read_dims(obj_dims_x, obj_dims_y);
  
  blockdim_x = obj_dims_x / tilesize_x + 2;
  blockdim_y = obj_dims_y / tilesize_y + 2;
  
  map_section = collision->get_mapsection(world_x, world_y, blockdim_x, 0);
 
  std::vector<cGameObject>::iterator iter;
  for (iter = map_section.begin(); iter != map_section.end(); ++iter) {
     if ( (COLLISION_STATUS = obj_collision(objectA, &(*iter))) != COLLISION_NULL )
      return COLLISION_STATUS;
  }
   return COLLISION_STATUS;
}
