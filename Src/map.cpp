#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "game.hpp"
#include "map.hpp"
#define QUEUE_SIZE 3000

const int block_width = 21,  block_height = 21;			
const int map_offset_x = 25, map_offset_y = 50;			
const int four_probe[4][2] = {{ 1, 0 }, { 0, 1 }, { -1,0 }, { 0, -1 }};
ALLEGRO_BITMAP* all;

const char* nthu_map[] = {
	"#####################################",
	"#...............F..###.........#####",
	"#.####.###########.....#######.....#",
	"#.####.#...........###.....# #.###.#",
	"#.P......#########.....#.#.###.# #.#",
	"#.#####.###      ###.###.#.#P#.# #.#",
	"#.#   #...#        #.###.#.#.#.# #.#",
	"#.#   #.#.#        #.###.#...#.# #.#",
	"#.#####.#.##########.###.#####.###.#",
	"#...............S..................#",
	"#.#######.########.##BBB##.##.####.#",
	"#.###  ##.########.##BBB##.##.#P##.#",
	"#.#### ##....##....##BBB##.##.#.##.#",
	"#.## ####.##.##.##.#######.##.#.##.#",
	"#.##  ###.##.##.##.#######.##...##.#",
	"#.##   ##.##.##.##.##   ##.#######.#",
	"#.#######.##.##.##.#######.#######.#",
	"#.........................P........#",
	"#.######.###.##########.######.###.#",
	"#.#    #.###.#####   #.........# #.#",
	"#.######.P.......#####.###.###.# #.#",
	"#........#######.......# #.# #.# #.#",
	"#.######.#     #####.### #.# #.###.#",
	"#.#    #.###########.#####.###.....#",
	"#.######.#.....P.............#.###.#",
	"#..........#########.#######.#.# #.#",
	"#.#######.##########.#     #...# #.#",
	"#.#######............#######.#.###.#",
	"#.........##########.........#.....#",
	"####################################"
};

const char* default_map[] = {
	"####################################",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                    BBB           #",
	"#                    BBB           #",
	"#                    BBB           #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"#                                  #",
	"####################################",
};


Map::Map(const char *filepath) {

	all = load_bitmap("Assets/pacman_tile.png"); 
	FILE* pFile = NULL;
	int n,m;
	if (filepath == NULL) {
		row_num = 30;
		col_num = 36;
		game_log("Creating from default map. row = %d col = %d", row_num, col_num);
		
	}
	else {
		game_log("%s\n", filepath);
		pFile = fopen("Assets/map_nthu.txt","r");
		if (!pFile)
			game_abort("error to open map file\n");
		if(fscanf(pFile,"%d %d",&n,&m) != 2)
			game_abort("Map format unmatched\n");
		row_num=n;	
		col_num=m;
		getc(pFile); // get the '\n'
	}
	map = (char**)malloc(sizeof(char*) * row_num);
	for (int i = 0; i < row_num; i++) {
		map[i] = (char*)malloc(sizeof(char) * col_num);
	}
	wallnum = beansCount = 0;
	for (int i = 0; i < row_num; i++) {
		for (int j = 0; j < col_num; j++) {
			if (filepath == NULL)
				map[i][j] = nthu_map[i][j];
			else{
				fscanf(pFile," %c",&map[i][j]);
			}		
			switch (map[i][j])
			{
			case '#':
				++wallnum;
				break;
			case '.':
			case 'P':
				++beansCount;
				break;
			default:
				break;
			}
		}
		if(filepath != NULL)
			getc(pFile);
	}
	beansNum = beansCount;
}

Map::~Map() {
	if (map) {
		for(int i = 0;i < col_num; i++)
			free(map[i]);
		free(map);
	}
}


void Map::draw() {
	for (int row = 0; row < row_num; row++) {
		for (int col = 0; col < col_num; col++) {
			switch (map[row][col])
			{
				case '#':
					draw_block_index(row, col);
					break;
					case 'P':
					draw_power_bean(row, col);
					break;
				case '.':
					draw_bean(row, col);
					break;
				case 'F':
					draw_cherry(row, col);
					break;
				case 'S':
					draw_strawberry(row, col);
					break;
				default:
					break;
			}
		}
	}
}
void Map::draw_cherry(const int row, const int col) {
	al_draw_scaled_bitmap(
		all,
		32, 48,
		16, 16,
		map_offset_x+col*block_height, map_offset_y+row*block_width,
		20, 20, 0
	);
}
void Map::draw_strawberry(const int row, const int col) {
	al_draw_scaled_bitmap(
		all,
		48, 48,
		16, 16,
		map_offset_x+col*block_height, map_offset_y+row*block_width,
		20, 20, 0
	);
}
void Map::draw_block_index(const int row, const int col) {
	bool U = is_wall_block(col, row - 1);
	bool UR = is_wall_block(col + 1, row - 1);
	bool UL = is_wall_block(col -1, row- 1);
	bool B = is_wall_block(col, row+ 1);
	bool BR = is_wall_block(col + 1, row + 1);
	bool BL = is_wall_block(col - 1, row + 1);
	bool R = is_wall_block(col + 1, row);
	bool L = is_wall_block(col - 1,row);
	if (!(U && UR && UL && B && BR && BL && R && L)) {
	int s_x, s_y, e_x, e_y, dw;
	int block_x = map_offset_x + block_width * col;
	int block_y = map_offset_y + block_height * row;
	dw = block_width / 3;
		s_x = block_x + dw;
		s_y = block_y+ dw;
		e_x = s_x + dw;
		e_y = s_y + dw;
		//25 154 25//rand()%256,rand()%256,rand()%256
		al_draw_filled_rectangle(s_x, s_y,
			e_x, e_y, al_map_rgb(25, 154, 25));
		if (row < row_num - 1 && B && !(BL && BR && R && L)) {
			s_x = block_x + dw;
			s_y = block_y + dw;
			e_x = s_x + dw;
			e_y = block_y + block_height;
			al_draw_filled_rectangle(s_x, s_y,
				e_x, e_y, al_map_rgb(25,154,25));
		}
		if (row > 0 && U && !(UL && UR && R && L)) {
			s_x = block_x + dw;
			s_y = block_y + (dw << 1);
			e_x = s_x + dw;
			e_y = block_y;
			al_draw_filled_rectangle(s_x, s_y,
				e_x, e_y, al_map_rgb(25,154 ,25));
		}
		if (col < col_num - 1 && R && !(UR && BR && U && B)) {
			s_x = block_x + dw;
			s_y = block_y + dw;
			e_x = block_x + block_width;
			e_y = s_y + dw;
			al_draw_filled_rectangle(s_x, s_y,
				e_x, e_y, al_map_rgb(25,154 ,25));

		}
		if (col > 0 && L && !(UL && BL && U && B)) {
			s_x = block_x;
			s_y = block_y + dw;
			e_x = s_x + (dw << 1);
			e_y = s_y + dw;
			al_draw_filled_rectangle(s_x, s_y,
				e_x, e_y, al_map_rgb(25,154 ,25));
		}
	}
}

void Map::draw_bean(const int row, const int col) {
	al_draw_filled_circle(map_offset_x + col * block_width + block_width / 2.0, map_offset_y + row * block_height + block_height / 2.0, block_width/6.0,  al_map_rgb(234, 38, 38));
}

void Map::draw_power_bean(const int row, const int col) {
	al_draw_filled_circle(map_offset_x + col * block_width + block_width / 2.0, map_offset_y + row * block_height + block_height / 2.0, block_width / 3.0, al_map_rgb(234, 178, 38));
}


bool Map::is_wall_block(int index_x, int index_y) {
	if (index_x < 0 || index_x >= col_num || index_y < 0 || index_y >= row_num)
		return true;
	return map[index_y][index_x] == '#';
}
bool Map::is_room_block(int index_x, int index_y) {
	if (index_x < 0 || index_x >= col_num || index_y < 0 || index_y >= row_num)
		return true;
	return map[index_y][index_x] == 'B';
}


Directions Map::shortest_path_direc(int startGridx, int startGridy, int endGridx, int endGridy) {

static int8_t queue_x[QUEUE_SIZE];
static int8_t queue_y[QUEUE_SIZE];
static	uint16_t front;
static	uint16_t end;

	static Directions steped[MAX_WALL_NUM_H][MAX_WALL_NUM_W];
	memset(steped, 0, sizeof(steped));
	front = end = 0;
	queue_x[end] = startGridx;
	queue_y[end] = startGridy;
	steped[startGridy][startGridx] = (Directions)1; 

	end++;

	for (size_t i = 0; i < 4; i++) {
		int8_t x = queue_x[front] + four_probe[i][0];
		int8_t y = queue_y[front] + four_probe[i][1];
		if (is_wall_block(x, y) || (int)steped[y][x])
			continue;
		queue_x[end] = x;
		queue_y[end] = y;
		switch (i) {
			case 0:
				steped[y][x] = Directions::RIGHT;
				break;
			case 1:
				steped[y][x] = Directions::DOWN;
				break;
			case 2:
				steped[y][x] = Directions::LEFT;
				break;
			case 3:
				steped[y][x] = Directions::UP;
				break;
			default:
				break;
		}
		end++;
	}
	front++;

	while (front != end && steped[endGridy][endGridx] == Directions::NONE) {

		for (size_t i = 0; i < 4; i++) {
			int8_t x = queue_x[front] + four_probe[i][0];
			int8_t y = queue_y[front] + four_probe[i][1];
			if (is_wall_block(x, y) || (int)steped[y][x])
				continue;
			queue_x[end] = x;
			queue_y[end] = y;
			steped[y][x] = steped[queue_y[front]][queue_x[front]];
			end++;
		}
		front++;
	}
	return steped[endGridy][endGridx];
}
