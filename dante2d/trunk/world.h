#ifndef INC_WORLD_H
#define INC_WORLD_H

#include <string>
#include <vector>
#include "texture.h"

class Tile : public cGameObject
{
 private:
  cGameObject* this_tile;
 protected:

 public:
  Tile(std::string name, bool);
  ~Tile();
  void draw();
 };

class TileMap
{
 private:
  std::vector<Sint16> the_tilemap;

  //dangerous, this is deleted by level data
  std::vector<Tile*> tileDefinitions;

  Sint16 scrollrate_x, scrollrate_y;
  Sint32 start_x, start_y;
  Sint32 tilesize_x, tilesize_y;
  Sint32 screen_blocks_x, screen_blocks_y;
  Sint32 mapsize_x, mapsize_y;
  bool is_playerlayer;
  Sint32 world_x, world_y;
  //Functions
  bool read_settings(std::string &);
 protected:

 public:
  TileMap(std::vector<Sint16> new_tilemap, std::vector<Tile*> new_tileDefintions,
		 Sint16 new_scrollrate_x, Sint16 new_scrollrate_y,
		 Sint32 new_start_x, Sint32 new_start_y,
		 Sint32 new_tilesize_x,Sint32 new_tilesize_y,
		 Sint32 new_mapsize_x, Sint32 new_mapsize_y,
		 bool new_isplayerlayer);

  ~TileMap();
  void draw();
  void print_map();
  void set_worldpos(const Sint32 &newposx, const Sint32 &newposy);
  void debug();
  std::vector<cGameObject> get_mapsection(Sint32, Sint32, Sint32, Sint32);
  void get_tilesize(Sint32 &, Sint32 &) const;
 };

class LevelData
{
 private:
  std::vector<std::vector<Sint16> > layer_defs;
  std::vector<TileMap*> layers;
  std::vector<Tile*> tileDefinitions;
  TileMap *collision_map;
  Uint16 num_layers;
  Sint32 tilesize_x, tilesize_y;
  Sint32 screen_blocks_x, screen_blocks_y;
  Sint32 mapsize_x, mapsize_y;
  Sint32 world_x, world_y;
  bool collision_debug;

 protected:
 public:
  static bool isScrollx_min, isScrollx_max;
  static bool isScrolly_min, isScrolly_max;
  static LevelData *current_level;
  LevelData();
  ~LevelData();
  void draw();
  bool read_settings(std::string level_name);
  Sint32 get_map_data_int(std::string &line, std::string search);
  std::string get_map_data_str(std::string &line, std::string search);
  void print();
  void Scroller(int, int);
  bool get_tile_defs(std::string t_defs);
  TileMap* get_collision_map();
  void get_world_max(Sint32 &, Sint32 &);
};

#endif  /* INC_WORLD_H */
