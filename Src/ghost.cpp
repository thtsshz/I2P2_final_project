#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>
#include "ghost.hpp"
#include "pacman_obj.hpp"
#include "map.hpp"
#include "game.hpp"
const int cage_grid_x=22, cage_grid_y=11;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* power_up_timer;
extern const int block_width,  block_height;
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
static int check=0;
static const int basic_speed = 2;

Ghost::Ghost()
{
	objData.Size.x = block_width;
	objData.Size.y = block_height;

	objData.nextTryMove = Directions::NONE;
	speed = basic_speed;
	status = GhostStatus::BLOCKED;
	
	flee_sprite = load_bitmap("Assets/ghost_flee.png");
	dead_sprite = load_bitmap("Assets/ghost_dead.png");

	previous_timer_val=0;
	objData.Coord.x = cage_grid_x;
	objData.Coord.y = cage_grid_y;
}
Ghost::~Ghost(){
	al_destroy_bitmap(dead_sprite);
	al_destroy_bitmap(flee_sprite);
	al_destroy_bitmap(move_sprite);
}
void Ghost::draw() {
	RecArea drawArea = getDrawArea(objData, GAME_TICK_CD);
	int v=objData.moveCD-objData.moveCD/32*32;
	int bitmap_x_offset = 0;
	check++;
	if(check==65)
		check=0;
	if (status == GhostStatus::FLEE){
		if(al_get_timer_count(power_up_timer)>7){
			if(check>32){
				if(v>16){
					al_draw_scaled_bitmap(flee_sprite,32,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				else{
					al_draw_scaled_bitmap(flee_sprite,48,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}	
			}
			else{
				if(v>16){
					al_draw_scaled_bitmap(flee_sprite,0,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				else{
					al_draw_scaled_bitmap(flee_sprite,16,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
			}
		}
		else{
			if(v>16){
				al_draw_scaled_bitmap(flee_sprite,0,0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
					//30
				);
			}
			else{
				al_draw_scaled_bitmap(flee_sprite,16,0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
					//30
				);
			}
		}
	}
	else if (status == GhostStatus::GO_IN) {
		switch (objData.facing){
			case Directions::RIGHT:
				al_draw_scaled_bitmap(dead_sprite,0,0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
					//30
				);
				break;
			case Directions::LEFT:
				al_draw_scaled_bitmap(dead_sprite,16,0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
					//30
				);
				break;
			case Directions::UP:
				al_draw_scaled_bitmap(dead_sprite,32,0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
					//30
				);
				break;
			default:
				al_draw_scaled_bitmap(dead_sprite,48,0,
					16, 16,
					drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
					draw_region, draw_region, 0
					//30
				);
				break;
		}
	}
	else {
		switch (objData.facing){ 
			case Directions::RIGHT:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite,0,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite,16,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				break;
			case Directions::LEFT:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite,32,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite,48,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				break;
			case Directions::UP:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite,64,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite,80,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				break;
			default:
				if(v>16){
					al_draw_scaled_bitmap(move_sprite,96,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				else{
					al_draw_scaled_bitmap(move_sprite,112,0,
						16, 16,
						drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
						draw_region, draw_region, 0
						//30
					);
				}
				break;
		}
	}

}
void Ghost::NextMove(Directions next) {
	objData.nextTryMove = next;
}
void Ghost::printGhostStatus(GhostStatus S) {

	switch(S){
	case GhostStatus::BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GhostStatus::GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case GhostStatus::FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GhostStatus::GO_IN:
		game_log("GO_IN");
		break;
	case GhostStatus::FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool Ghost::movable(Map* M, Directions targetDirec, bool room) {
	int x=objData.Coord.x,y=objData.Coord.y;
	switch (targetDirec)
	{
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

	if (M->is_wall_block(x, y) || (room && M->is_room_block(x, y)))
		return false;
	return true;

}

void Ghost::toggle_FLEE(bool setFLEE) {
	if(setFLEE){
		if(status == GhostStatus::FREEDOM){ 
			status = GhostStatus::FLEE;
			speed = 1;
		}
	}else{
		if(status == GhostStatus::FLEE){
			status = GhostStatus::GO_IN;
			speed=4;
		}
	}
}

void Ghost::collided() {
	if (status == GhostStatus::FLEE) {
		status = GhostStatus::GO_IN;
		speed = 4;
	}
}

void Ghost::move_script_GO_IN(Map* M) {
	objData.nextTryMove = M->shortest_path_direc(objData.Coord.x, objData.Coord.y, cage_grid_x, cage_grid_y + 1);
}
void Ghost::move_script_GO_OUT(Map* M) {
	if(M->map[objData.Coord.y][objData.Coord.x] == 'B') 
		NextMove(Directions::UP);
	else
		status = GhostStatus::FREEDOM;
}
int inv2(int dir){
	if(dir==(int)Directions::UP)
		return (int)Directions::DOWN;
	if(dir==(int)Directions::DOWN)
		return (int)Directions::UP;
	if(dir==(int)Directions::LEFT)
		return (int)Directions::RIGHT;
	return (int)Directions::LEFT;
} 
void Ghost::move_script_FLEE(Map* M, const Pacman * const pacman) {
	Directions shortestDirection = M->shortest_path_direc(objData.Coord.x, objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
	static int proba[4]; // possible movement
	int i,cnt = 0;
	for (i = 1; i <= 4; i++)
		if (movable(M,(Directions)i,1)&&i!=(int)shortestDirection&&inv2(i)!=(int)objData.preMove)//
		 	proba[cnt++] = i;
	if(cnt){
		NextMove((Directions)proba[rand()%cnt]);
		return;
	}	
	NextMove((Directions)inv2((int)objData.preMove));
}

void Ghost::move(Map *M) {
	if (!movetime(speed))
		return;
	int probe_x = objData.Coord.x, probe_y = objData.Coord.y;
	if (movable(M, objData.nextTryMove, false)) 
		objData.preMove = objData.nextTryMove;
	else if (!movable(M, objData.preMove, false)) 
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

GhostRed::GhostRed() : Ghost()
{
	move_sprite = load_bitmap("Assets/ghost_move_red.png");
}

GhostPink::GhostPink() : Ghost()
{
	move_sprite = load_bitmap("Assets/ghost_move_pink.png");
}

GhostBlue::GhostBlue() : Ghost()
{
	move_sprite = load_bitmap("Assets/ghost_move_blue.png");
}

GhostOrange::GhostOrange() : Ghost()
{
	move_sprite = load_bitmap("Assets/ghost_move_orange.png");
}
