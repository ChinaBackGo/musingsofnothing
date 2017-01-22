#include "world.h"
#include "sprite.h"
#include "texture.h"
#include "assert.h"
#include "engine.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool LevelData::isScrollx_min = false;
bool LevelData::isScrolly_min = false;
bool LevelData::isScrollx_max = false;
bool LevelData::isScrolly_max = false;
LevelData* LevelData::current_level = 0;

///////////////////////
//TILE CLASS
///////////////////////
Tile::Tile(std::string name, bool animated) {
  //  tex(name.c_str());
  if(animated)
    this_tile = new animSprite(name, 0, 0);
  else
    this_tile = new Texture(name, 0, 0);
}
Tile::~Tile() {
  delete this_tile;
}

void Tile::draw() {
  this_tile->set_pos(position.x, position.y);
  this_tile->draw();
}

/////////////////////////
//TileMap Class
/////////////////////////
TileMap::TileMap(std::vector<Sint16> new_tilemap, std::vector<Tile*> new_tileDefintions,
		 Sint16 new_scrollrate_x, Sint16 new_scrollrate_y,
		 Sint32 new_start_x, Sint32 new_start_y,
		 Sint32 new_tilesize_x,Sint32 new_tilesize_y,
		 Sint32 new_mapsize_x, Sint32 new_mapsize_y,
		 bool new_isplayerlayer)
  :scrollrate_x(new_scrollrate_x), scrollrate_y(new_scrollrate_y),
   start_x(new_start_x), start_y(new_start_y),
   tilesize_x(new_tilesize_x), tilesize_y(new_tilesize_y),
   mapsize_x(new_mapsize_x), mapsize_y(new_mapsize_y),
   is_playerlayer(new_isplayerlayer), world_x(start_x), world_y(start_y) {
  
  //  if(!read_settings(tiledata)) {
//      std::cout << "TileMap could not be read" << std::endl;
//      assert(false);
  // }  

  assert(start_x >= 0);
  assert(start_y >= 0);
  assert(tilesize_x % 8 == 0);
  assert(tilesize_y % 8 == 0);
  assert(mapsize_x > 0);
  assert(mapsize_y > 0);

  the_tilemap = new_tilemap;
  tileDefinitions = new_tileDefintions;
  world_x = start_x;
  world_y = start_y;
  screen_blocks_x = (Engine::SCREEN_W)/tilesize_x +1;
  screen_blocks_y = (Engine::SCREEN_H)/tilesize_y +1;

  assert(mapsize_x > screen_blocks_x);
  assert(mapsize_y > screen_blocks_y);
}
TileMap::~TileMap() {
  //  vector<Tile*>::iterator iter;

//   for (iter = tileDefinitions.begin(); iter != tileDefinitions.end(); ++iter)
//    delete *iter;
}
bool TileMap::read_settings(std::string &tiledata) {

  //Tile Map load
  string line;
  string tiledefs;
  ifstream tiledata_file(tiledata.c_str());

  if(!tiledata_file) {
    return false;
  }

  if(getline(tiledata_file, line)) {
    istringstream stream(line);
    //Get the tile defs file
    stream >> tiledefs;
    if(!stream)
      return false;
  }

  if(getline(tiledata_file, line)) {
    istringstream stream(line);
    //Get the starting block
    stream >> start_x >> start_y;
    if(!stream)
      return false;
  }

  if(getline(tiledata_file, line)) {
    istringstream stream(line);
    //Get the tilesize
    stream >> tilesize_x >> tilesize_y;
    if(!stream)
      return false;
  }

  if(getline(tiledata_file, line)) {
    istringstream stream(line);
    //Get the map dimensions
    stream >> mapsize_x >> mapsize_y;
    if(!stream)
      return false;
  }

  while(getline(tiledata_file, line)) {
    istringstream stream(line);
    Sint16 t_sint16;
    //Get the map info
    while(stream >> t_sint16)
      the_tilemap.push_back(t_sint16);
  }

  //while...get tile info
  tiledata_file.close();



  //load tiledefs
  ifstream tiledefs_file(tiledefs.c_str());

  line.clear();
  if(!tiledefs_file) {
    return false;
  }

  while(getline(tiledefs_file, line)) {
    istringstream stream(line);
    string tilename;
    bool b_isanim;

    //tilefile animation
    stream >> tilename >> b_isanim;
    tilename = "maps/tiles/" + tilename;
    Tile *new_tile = new Tile(tilename, b_isanim);
    //load up tile definitions
    tileDefinitions.push_back( new_tile );
  }
  tiledefs_file.close();



  //load tilemap
  return true;
}
void TileMap::get_tilesize(Sint32 &read_tilesize_x, Sint32 &read_tilesize_y) const{
  read_tilesize_x = tilesize_x;
  read_tilesize_y = tilesize_y;
}
void TileMap::draw() {
  Sint32 x_offset, y_offset;
  Sint32 block_x, block_y;

  //Set our offsets
  x_offset = (world_x % tilesize_x)*-1;
  y_offset = (world_y % tilesize_y)*-1;

  //set our block position
  block_x = world_x / tilesize_x;
  //  block_y = world_y / tilesize_y;
  block_y = mapsize_y - world_y / tilesize_y - 1;


  //indices
  Sint32 ibx,iby, screen_x, screen_y;
  screen_x = 0;
  screen_y = 0;

  for(ibx = block_x; ibx < screen_blocks_x + block_x; ibx++) {
    //    for(iby = block_y; iby < screen_blocks_y + block_y; iby++) {
    for(iby = block_y; iby > block_y - screen_blocks_y - 1; iby--) {

      //Make sure we don't go out of bounds
      if( (Uint32)(ibx + iby*mapsize_x) > (the_tilemap.size() - 1) )  {
	cout << "ERROR: Map tried to scroll out of bounds" << endl;
	cout << "DEBUG INFO world_x: " << world_x << " world_y: " << world_y << endl;
	assert(false);
      }

      Uint16 tile_id = the_tilemap[ibx + iby*mapsize_x];
      if(tile_id !=0) {
	Tile *temp_tile = tileDefinitions[tile_id-1];
	temp_tile->set_pos( x_offset + (tilesize_x*screen_x),
			    y_offset + (tilesize_y*screen_y) );
	temp_tile->draw();
      }
      screen_y++;
    }
    screen_x++;
    screen_y = 0;
  }
}

std::vector<cGameObject> TileMap::get_mapsection(Sint32 point_x, Sint32 point_y,
					    Sint32 dimx, Sint32 dimy) {

  std::vector<cGameObject> map_section;
  Sint32 block_x, block_y;

  //set our block position
  block_x = point_x / tilesize_x;
  //  block_y = point_y / tilesize_y;
  block_y = mapsize_y - point_y / tilesize_y - 1;


  //indices
  Sint32 ibx,iby, screen_x, screen_y;
  screen_x = 0;
  screen_y = 0;
  
  for(ibx = block_x; ibx < dimx + block_x; ibx++) {
    for(iby = block_y; iby > block_y - dimy - 1; iby--) {

      //Make sure we don't go out of bounds
      if( (Uint32)(ibx + iby*mapsize_x) > (the_tilemap.size() - 1) )  {
	cout << "ERROR: Map tried to scroll out of bounds MAP SECTION" << endl;
	cout << "DEBUG INFO point_x: " << point_x << " point_y: " << point_y << endl;
	assert(false);
      }

      Uint16 tile_id = the_tilemap[ibx + iby*mapsize_x];
      if (tile_id != 0){
	Sint32 temp_ibx = ibx*tilesize_x;
	Sint32 temp_iby = ((mapsize_y-1)-iby)*tilesize_y;
	cGameObject temp_tile(temp_ibx, temp_iby, tilesize_x, tilesize_y);
	temp_tile.set_world_pos(temp_ibx, temp_iby);
	map_section.push_back(temp_tile);
      }
      screen_y++;
    }
    screen_x++;
    screen_y = 0;
  }
  return map_section;
}

void TileMap::print_map() {
  vector<Sint16>::iterator iter;
  for (iter = the_tilemap.begin(); iter != the_tilemap.end(); ++iter)
    cout << *iter;

  cout << endl;
}
void TileMap::set_worldpos(const Sint32 &newposx, const Sint32 &newposy) {
  world_x = newposx;
  world_y = newposy;
}

void TileMap::debug() {
  cout << "Scrollrate x:" << scrollrate_x << " y:" << scrollrate_y << endl;
  cout << "Start x:" << start_x << " y:" << start_y << endl;
  cout << "Tilesize x:" << tilesize_x << " y:" << tilesize_y << endl;
  cout << "Screen Blocks x:" << screen_blocks_x << " y:" << screen_blocks_y << endl;
  cout << "Mapsize x:" << mapsize_x << " y:" << mapsize_y << endl;
  cout << "World x:" << world_x << " y:" << world_y << endl;
  cout << "Player Layer :" << is_playerlayer << endl;

  print_map();
}

LevelData::LevelData()
  :world_x(0), world_y(0) {
  collision_debug = true;
}
LevelData::~LevelData() {
  //Clean up our tile memory

  delete collision_map;

  vector<TileMap*>::iterator iter2;
  for (iter2 = layers.begin(); iter2 != layers.end(); ++iter2)
    delete *iter2;

  vector<Tile*>::iterator iter;
  for (iter = tileDefinitions.begin(); iter != tileDefinitions.end(); ++iter)
    delete *iter;


}
bool LevelData::read_settings(std::string level_name) {
  //Tile Map load
  string line;
  string map;
  string tiledefs;
  ifstream leveldata_file(level_name.c_str());


  if(!leveldata_file) {
    return false;
  }

  while(getline(leveldata_file, line)) {
    istringstream stream(line);
    string dump;
    if ( line.find("<map", 0) != string::npos ) {
      //      cout << "MAP: " << line << endl;
      mapsize_x = get_map_data_int(line, "width");
      mapsize_y = get_map_data_int(line, "height");
      tilesize_x = get_map_data_int(line, "tilewidth");
      tilesize_y = get_map_data_int(line, "tileheight");
      assert(mapsize_x > 0);
      assert(mapsize_y > 0);
      assert(tilesize_x > 0);
      assert(tilesize_y > 0);

    } else if ( line.find("<tileset", 0) != string::npos ) {
      //      cout << "TILESET :" << line << endl;
      tiledefs = get_map_data_str(line, "source");

      //Load our tiledefintions before we can push layers
      get_tile_defs(tiledefs);

    } else if ( line.find("<layer", 0) != string::npos ) {
      //      cout << "LAYER: " << line << endl;
      bool isCollision = false;

      //new Layer
      std::vector<Sint16> new_layer;
      while(getline(leveldata_file, line)) {
	if ( line.find("<property", 0) != string::npos ){
	  if ( line.find("isCollision", 0) != string::npos ){
	    isCollision = true;
	    cout << "We have collision layer" << endl;
	  }
	}

	if ( line.find("<tile", 0) != string::npos ) {
	  
	  //	  cout << "TILE: " << line << endl;
	  //Get the Tile ID
	  new_layer.push_back(get_map_data_int(line, "gid"));
	  //Get tile value add to vector
	  //get line till </Layer
	}
	if ( line.find("</layer", 0) != string::npos ) {
	  //Add our layer
	  //layer_defs.push_back(new_layer);
	  
	  //Make our layer
	  TileMap *new_TileMap = new TileMap(new_layer, tileDefinitions,
					     0, 0, //0 scroll rate for now
					     0, 0, //0 start for now
					     tilesize_x, tilesize_y,
					     mapsize_x, mapsize_y,
					     isCollision); //No player layer yet.
 	  if(isCollision)
 	    collision_map = new_TileMap;
 	  else
	    layers.push_back(new_TileMap);

	  break;
	}
      }
    }
  }
  leveldata_file.close();

  assert(collision_map != 0);

  return true;
}

bool LevelData::get_tile_defs(string t_defs) {

string tiledefs = "maps/" + t_defs;
  string line;
  ifstream tiledefs_file(tiledefs.c_str());  

  //Load the tildefs vector
  if(!tiledefs_file) {
    return false;
  }

  while(getline(tiledefs_file, line)) {
    istringstream stream(line);
    string tilename;
    //    bool b_isanim;
    if ( line.find("<property", 0) != string::npos ) {

      tilename = get_map_data_str(line, "value");

      tilename = "maps/tiles/" + tilename;
      Tile *new_tile = new Tile(tilename, false);
    //load up tile definitions
      tileDefinitions.push_back( new_tile );
    }
  }
  tiledefs_file.close();
  return true;
}
Sint32 LevelData::get_map_data_int(std::string &line, std::string search) {
  //  Sint32 mapx, mapy, tilex, tiley;
  int index1 = 0;
  int index2 = 0;
  Sint32 answer;
  string search1 = search + "=\"";
  string result;
  index1 = line.find(search1, 0) + search1.length();
  index2 = line.find("\"", index1);
  result =  line.substr(index1, index2-index1);
  
  istringstream stream(result);
  stream >> answer;
  return answer;
}

std::string LevelData::get_map_data_str(std::string &line, std::string search) {
  //  Sint32 mapx, mapy, tilex, tiley;
  int index1 = 0;
  int index2 = 0;
  string search1 = search + "=\"";
  string result;
  index1 = line.find(search1, 0) + search1.length();
  index2 = line.find("\"", index1);
  result =  line.substr(index1, index2-index1);
  return result;
}

//Level Data debugs??
void LevelData::print() {

  vector<TileMap*>::iterator iter;
  for(iter = layers.begin(); iter != layers.end(); ++iter) {
    (*iter)->debug();
  }

}

void LevelData::draw() {
  vector<TileMap*>::iterator iter;
  for(iter = layers.begin(); iter != layers.end(); ++iter) {
    (*iter)->set_worldpos(world_x, world_y);
    (*iter)->draw();
  }
  //Update our collision map position
  collision_map->set_worldpos(world_x, world_y);

  if (collision_debug)
    collision_map->draw();

}

void LevelData::Scroller(Sint32 scroll_x, Sint32 scroll_y) {
  Sint32 max_x, max_y;
  Sint32 min_x, min_y;


  min_x = Engine::SCREEN_W/2;
  min_y = Engine::SCREEN_H/2;
  max_x = (mapsize_x - 1)*tilesize_x - min_x;
  max_y = (mapsize_y - 1)*tilesize_y - min_y;

  assert(max_x > 0);
  assert(max_y > 0);
  assert(min_x > 0);
  assert(min_y > 0);
  
  if (scroll_x < min_x) {
      isScrollx_min = false;
      isScrollx_max = true;
      world_x = 0;
  } else if (scroll_x > max_x) {
      isScrollx_max = false;
      isScrollx_min = true;
      //maximum world X value without over scroll
      world_x = max_x - min_x - (Engine::SCREEN_W % tilesize_x) - 1;
  }
  else {
    world_x = scroll_x - min_x;
      isScrollx_max = true;
      isScrollx_min = true;
  }

  if (scroll_y < min_y) {
      isScrolly_min = false;
      isScrolly_max = true;
      world_y = 0;
  } else if (scroll_y > max_y) {
      isScrolly_min = true;
      isScrolly_max = false;
      //maximum world Y value without over scroll
      world_y =  max_y - min_y - (Engine::SCREEN_H % tilesize_y) - 1;
  }
  else {
    world_y = scroll_y - min_y;
      isScrolly_min = true;
      isScrolly_max = true;
  }


}

TileMap* LevelData::get_collision_map() {
  assert(collision_map != 0);
  return collision_map;
}

void LevelData::get_world_max(Sint32 &max_x, Sint32 &max_y) {
  max_x = mapsize_x*tilesize_x;
  max_y = mapsize_y*tilesize_y;
  
  assert (max_x > 0);
  assert (max_y > 0);
}
