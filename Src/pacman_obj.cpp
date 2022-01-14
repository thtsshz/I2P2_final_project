#include <allegro5/allegro_primitives.h>
#include "pacman_obj.h"
#include "map.h"
#include "game.h" 
#include "shared.h"
 
/* Static variables */
static const int start_grid_x = 25, start_grid_y = 25;		// where to put pacman at the beginning
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;  // draw offset 
static const int draw_region = 30;							// pacman bitmap draw region
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
// [ NOTE - speed ]
// If you want to implement something regarding speed.
// You may have to modify the basic_speed here.
// But before you modify this, make sure you are 
// totally understand the meaning of speed and function
// `step()` in `scene_game.c`, also the relationship between
// `speed`, `GAME_TICK`, `GAME_TICK_CD`, `objData->moveCD`.
static const int basic_speed = 2;

/* Shared variables */
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern uint32_t GAME_TICK;
//extern uint32_t GAME_TICK_CD;
extern bool game_over;
extern bool check_color;
extern float effect_volume;
extern bool check_color;
/* Declare static function */
bool Pacman::movable(Map *M, Directions targetDirec) {
	// [HACKATHON 1-2]
	// TODO: Determine if the current direction is movable.
	// That is to say, your pacman shouldn't penetrate 'wall' and 'room'
	// , where room is reserved for ghost to set up.
	// 1) For the current direction `targetDirec`, use pre-implemented function
	// `is_wall_block` and `is_room_block` to check if the block is wall or room. (they are both defined in map.c)
	// 2) the coordinate data of pacman is stored in objData.Coord
	// it is a self-defined pair IntInt type. Trace the code and utilize it.

	/*
	... objData.Coord.x, ... objData.Coord.y;
	*/
	int x = objData.Coord.x, y = objData.Coord.y;
	switch (targetDirec){
		case Directions::UP:
			y--;
			break;
		case Directions::DOWN:
			y++;
			break;
		case Directions::LEFT:
			x--;
			break;
		case Directions::RIGHT:
			x++;
			break;
		default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
			return false;
	}
	if (is_wall_block(M, x, y) || is_room_block(M, x, y))
		return false;
	
	return true;
}

Pacman::Pacman() {

	/*
		[TODO]
		Allocate dynamic memory for pman pointer;
	*/

	/*
		Pacman* pman = ...
		if(!pman)
			return NULL;
	*/
	/* Init pman data */
	/* set starting point, Size, */
	/* TODO? */
	/* hint / just put it */
	objData.Coord.x = 24;
	objData.Coord.y = 24;
	objData.Size.x = block_width;
	objData.Size.y = block_height;

	objData.preMove = Directions::NONE;
	objData.nextTryMove = Directions::NONE;
	speed = basic_speed;

	death_anim_counter = al_create_timer(1.0 / 64);
	powerUp = false;
	/* load sprites */
	if(check_color){
		move_sprite = load_bitmap("Assets/pacman_move2.png");
		die_sprite = load_bitmap("Assets/pacman_die2.png");
	}
	else{
		move_sprite = load_bitmap("Assets/pacman_move.png");
		die_sprite = load_bitmap("Assets/pacman_die.png");
	}

}

Pacman::~Pacman() {
	/*[TODO]
	free pacman resource*/
	al_destroy_bitmap(die_sprite);
	al_destroy_bitmap(move_sprite);
	al_destroy_timer(death_anim_counter);
}

void Pacman::draw() {
	/*
		[HW-TODO ]
		Draw Pacman and animations
		hint: use objData.moveCD to determine which frame of the animation to draw, you may refer to discription in ghost_draw in ghost.c
	*/
	if(tran){
		tran=0;
		objData.Coord.x = 25;
		objData.Coord.y = 24;
	} 
	RecArea drawArea = getDrawArea(objData, GAME_TICK_CD);
	
	//Draw default image
//	al_draw_scaled_bitmap(move_sprite, 0, 0,
//		16, 16,
//		drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
//		draw_region, draw_region, 0
//	);
//	
	int offset = 0;
	if (game_over) {
		
		/*
			hint: instead of using objData.moveCD, use Pacman's death_anim_counter to create animation
		*/
		// 16 * 192 
		// 192 / 16 = 12
		int v = al_get_timer_count(death_anim_counter);
//		printf("timer: %d\n",v);
		al_draw_scaled_bitmap(die_sprite, v/12*16, 0,
			16, 16,
			drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
			draw_region, draw_region, 0
		);
	}
	else {
		int v = objData.moveCD - objData.moveCD / 32 * 32;
		switch (objData.facing) {
			case Directions::RIGHT:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite, 0, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite, 16, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				break;
			case Directions::LEFT:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite, 32, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite, 48, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				break;
			case Directions::UP:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite, 64, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite, 80, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				break;
			case Directions::DOWN:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite, 96, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite, 112, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				break;
			default:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite, 32, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite, 48, 0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
					);
				}
				break;
		}
	}
}
void Pacman::move(Map* M) {
	if (!movetime(speed))
		return;
	if (game_over)
		return;

	int probe_x = objData.Coord.x, probe_y = objData.Coord.y;
	if (movable(M, objData.nextTryMove)) 
		objData.preMove = objData.nextTryMove;
	else if (!movable(M, objData.preMove)) 
		return;
//	printf("MOVE DIRECTION%d\n",objData.preMove);
	switch (objData.preMove)
	{
	case Directions::UP:
		objData.Coord.y -= 1;
		objData.preMove = Directions::UP;
		break;
	case Directions::DOWN:
		objData.Coord.y += 1;
		objData.preMove = Directions::DOWN;
		break;
	case Directions::LEFT:
		objData.Coord.x -= 1;
		objData.preMove = Directions::LEFT;
		break;
	case Directions::RIGHT:
		objData.Coord.x += 1;
		objData.preMove = Directions::RIGHT;
		break;
	default:
		break;
	}
	objData.facing = objData.preMove;
	objData.moveCD = GAME_TICK_CD;
}
void Pacman::eatItem(const char Item) {
	switch (Item)
	{
	case '.':
		stop_bgm(PACMAN_MOVESOUND_ID);
		PACMAN_MOVESOUND_ID = play_audio(PACMAN_MOVESOUND, effect_volume);
		break;
	case 'F':
		back=1;
		break;
	case 'S':
		tran=1;
		break;
	default:
		break;
	}
}

void Pacman::NextMove(Directions next) {
	objData.nextTryMove = next;
}

void Pacman::die() {
//	puts("***");
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}





