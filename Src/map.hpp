#ifndef MAP_H
#define MAP_H
#include "utility.h"

#define MAX_WALL_NUM_W 100
#define MAX_WALL_NUM_H 100
extern const int block_width,  block_height;
extern const int map_offset_x, map_offset_y ;
class Map
{
private:
	int wallnum;
	int beansCount;
	void draw_cherry(const int, const int);
	void draw_strawberry(const int, const int);
	void draw_block_index(int, int);
	void draw_bean(const int, const int);
	void draw_power_bean(const int, const int);
public:
	int row_num, col_num;
	int beansNum;
	char** map;
	Map(const char * const);
	~Map();
	void draw();
	Directions shortest_path_direc(int, int, int, int);
	bool is_wall_block(int, int);
	bool is_room_block(int, int);
	bool is_wall_point(float, float);
	bool is_room_point(float, float);
};

//void draw_block(Map*, RecArea);

#endif
