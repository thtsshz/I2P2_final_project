
#include "ghost.h"
#include "pacman_obj.h"
#include "map.h"
/* Shared variables */
#define GO_OUT_TIME 256
extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern const int cage_grid_x, cage_grid_y;//22 11
/* Declare static function prototypes */
static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M,Pacman* pacman);
static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M);
int inv(int dir){
	if(dir==(int)Directions::UP)
		return (int)Directions::DOWN;
	if(dir==(int)Directions::DOWN)
		return (int)Directions::UP;
	if(dir==(int)Directions::LEFT)
		return (int)Directions::RIGHT;
	return (int)Directions::LEFT;
} 
static void ghost_red_move_script_FREEDOM(Ghost* ghost, Map* M,Pacman* pacman) {
	// [HACKATHON 2-4]
	// Uncomment the following code and finish pacman picking random direction.

	static Directions proba[4]; // possible movement
	int i,cnt = 0;
	for (i = 1; i <= 4; i++)
		if (ghost_movable(ghost,M,(Directions)i,1)&&inv(i)!=(int)ghost->objData.preMove)
		 	proba[cnt++] = (Directions)i;
	if(!cnt){
		ghost_NextMove(ghost,(Directions)inv((int)ghost->objData.preMove));
		return;
	}
	int x_dif=ghost->objData.Coord.x-pacman->objData.Coord.x,y_dif=ghost->objData.Coord.y-pacman->objData.Coord.y;
//	printf("%d %d\n",pacman->objData.Coord.x,pacman->objData.Coord.y);
	if(abs(x_dif)>abs(y_dif)){
		if(x_dif>0){
			for(i=0;i<cnt;i++)
				if(proba[i]==Directions::LEFT){
					ghost_NextMove(ghost,Directions::LEFT);
					return;
				}	
		}
		else{
			for(i=0;i<cnt;i++)
				if(proba[i]==Directions::RIGHT){
					ghost_NextMove(ghost,Directions::RIGHT);
					return;
				}
		}
	}
	else{
		if(y_dif>0){
			for(i=0;i<cnt;i++)
				if(proba[i]==Directions::UP){
					ghost_NextMove(ghost,Directions::UP);
					return;
				}	
		}
		else{
			for(i=0;i<cnt;i++)
				if(proba[i]==Directions::DOWN){
					ghost_NextMove(ghost,Directions::DOWN);
					return;
				}
		}
	}
	if(x_dif>0){
		for(i=0;i<cnt;i++)
			if(proba[i]==Directions::LEFT){
				ghost_NextMove(ghost,Directions::LEFT);
				return;
			}	
	}
	else{
		for(i=0;i<cnt;i++)
			if(proba[i]==Directions::RIGHT){
				ghost_NextMove(ghost,Directions::RIGHT);
				return;
			}
	}
	if(y_dif>0){
			for(i=0;i<cnt;i++)
				if(proba[i]==Directions::UP){
					ghost_NextMove(ghost,Directions::UP);
					return;
				}	
	}
	else{
		for(i=0;i<cnt;i++)
			if(proba[i]==Directions::DOWN){
				ghost_NextMove(ghost,Directions::DOWN);
				return;
			}
	}
	ghost_NextMove(ghost, proba[rand()%cnt]);
	// [TODO] (Not in Hackathon) 
	// Description:
	// For red(Blinky) ghost, we ask you to implement an random strategy ghost, 
	// which means moving in random direction.
	// But your random strategy have to designed carefully so that ghost won't walk back and forth.
	// (The code here DO perform walking back and forth.)
	
}

static void ghost_red_move_script_BLOCKED(Ghost* ghost, Map* M) {
	switch (ghost->objData.preMove)
	{
	case Directions::UP:
		if (ghost->objData.Coord.y == 10)
			ghost_NextMove(ghost, Directions::DOWN);
		else
			ghost_NextMove(ghost, Directions::UP);
		break;
	case Directions::DOWN:
		if (ghost->objData.Coord.y == 12)
			ghost_NextMove(ghost, Directions::UP);
		else
			ghost_NextMove(ghost, Directions::DOWN);
		break;
	default:
		ghost_NextMove(ghost, Directions::UP);
		break;
	}
}

void ghost_red_move_script(Ghost* ghost, Map* M, Pacman* pacman) {
	if (!movetime(ghost->speed))
		return;
		switch (ghost->status)
		{
		case GhostStatus::BLOCKED:
			ghost_red_move_script_BLOCKED(ghost, M);
//			printf("NOTICE:%d\n",al_get_timer_count(game_tick_timer)-ghost->previous_timer_val);
			if(!ghost->previous_timer_val){
				if(ghost->typeFlag==GhostType::Blinky){
					if (al_get_timer_count(game_tick_timer) >0)
						ghost->status = GhostStatus::GO_OUT;
				}
				else if(ghost->typeFlag==GhostType::Pinky){
					if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
						ghost->status = GhostStatus::GO_OUT;
				}
				else if(ghost->typeFlag==GhostType::Inky){
					if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME*3)
						ghost->status = GhostStatus::GO_OUT;
				}
				else{
					if (al_get_timer_count(game_tick_timer) > GO_OUT_TIME*5)
						ghost->status = GhostStatus::GO_OUT;
				}
			}
			else{
				if(al_get_timer_count(game_tick_timer)-ghost->previous_timer_val>GO_OUT_TIME*2){
					ghost->status = GhostStatus::GO_OUT;
				}
			}
			break;
		case GhostStatus::FREEDOM:
			ghost_red_move_script_FREEDOM(ghost, M,pacman);
			break;
		case GhostStatus::GO_OUT:
			ghost_move_script_GO_OUT(ghost, M);
			break;
		case GhostStatus::GO_IN:
			ghost_move_script_GO_IN(ghost, M);
			if (M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B') {
				ghost->status = GhostStatus::BLOCKED;
				ghost->speed = 2;
			}
			break;
		case GhostStatus::FLEE:
			ghost_move_script_FLEE(ghost, M, pacman);
			break;
		default:
			break;
		}

		if(ghost_movable(ghost, M, ghost->objData.nextTryMove, false)){
			ghost->objData.preMove = ghost->objData.nextTryMove;
			ghost->objData.nextTryMove = Directions::NONE;
		}
		else if (!ghost_movable(ghost, M, ghost->objData.preMove, false))
			return;
//		if(ghost->typeFlag==Pinky)
//			printf("COOD::%d %d\n",ghost->objData.Coord.x,ghost->objData.Coord.y);
		switch (ghost->objData.preMove) {
		case Directions::RIGHT:
			ghost->objData.Coord.x += 1;
			break;
		case Directions::LEFT:
			ghost->objData.Coord.x -= 1;
			break;
		case Directions::UP:
			ghost->objData.Coord.y -= 1;
			break;
		case Directions::DOWN:
			ghost->objData.Coord.y += 1;
			break;
		default:
			break;
		}
		ghost->objData.facing = ghost->objData.preMove;
		ghost->objData.moveCD = GAME_TICK_CD;
}
