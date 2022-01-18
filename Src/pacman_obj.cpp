#include <allegro5/allegro_primitives.h>
#include "pacman_obj.hpp"
#include "map.hpp"
#include "game.hpp" 
#include "shared.h"
 
static const int start_grid_x = 25, start_grid_y = 25;	
static const int fix_draw_pixel_offset_x = -3, fix_draw_pixel_offset_y = -3;   
static const int draw_region = 30;						
static ALLEGRO_SAMPLE_ID PACMAN_MOVESOUND_ID;
static const int basic_speed = 2;

extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern uint32_t GAME_TICK;
extern bool game_over;
extern bool check_color;
extern float effect_volume;
extern bool check_color;
bool Pacman::movable(Map *M, Directions targetDirec) {
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
			return false;
	}
	if (M->is_wall_block(x, y) || M->is_room_block(x, y))
		return false;
	
	return true;
}

Pacman::Pacman() {

objData.Coord.x = 24;
	objData.Coord.y = 24;
	objData.Size.x = block_width;
	objData.Size.y = block_height;

	objData.preMove = Directions::NONE;
	objData.nextTryMove = Directions::NONE;
	speed = basic_speed;

	death_anim_counter = al_create_timer(1.0 / 64);
	powerUp = false;
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
	al_destroy_bitmap(die_sprite);
	al_destroy_bitmap(move_sprite);
	al_destroy_timer(death_anim_counter);
}

void Pacman::draw() {
	if(tran){
		tran=0;
		objData.Coord.x = 25;
		objData.Coord.y = 24;
	} 
	RecArea drawArea = getDrawArea(objData, GAME_TICK_CD);
	

	int offset = 0;
	if (game_over) {
		// 16 * 192 
		// 192 / 16 = 12
		int v = al_get_timer_count(death_anim_counter);
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
	stop_bgm(PACMAN_MOVESOUND_ID);
	PACMAN_MOVESOUND_ID = play_audio(PACMAN_DEATH_SOUND, effect_volume);
}





