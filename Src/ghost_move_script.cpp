
#include "ghost.hpp"
#include "pacman_obj.hpp"
#include "map.hpp"
#include "game.hpp"
/* Shared variables */
#define GO_OUT_TIME 256
//extern uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER *game_tick_timer;
extern Ghost** ghosts;
extern const int cage_grid_x, cage_grid_y; //22 11
/* Declare static function prototypes */
Directions inv(Directions dir)
{
	if (dir == Directions::UP)
		return Directions::DOWN;
	if (dir == Directions::DOWN)
		return Directions::UP;
	if (dir == Directions::LEFT)
		return Directions::RIGHT;
	return Directions::LEFT;
}

void GhostRed::move_script_FREEDOM(Map *M, Pacman *pacman)
{
	// [HACKATHON 2-4]
	// Uncomment the following code and finish pacman picking random direction.

	static Directions proba[4]; // possible movement
	int i, cnt = 0;
	for (i = 1; i <= 4; i++)
		if (movable(M, (Directions)i, 1) && inv((Directions)i) != objData.preMove)
			proba[cnt++] = (Directions)i;
	if (!cnt)
	{
		NextMove(inv(objData.preMove));
		return;
	}
	int x_dif = objData.Coord.x - pacman->objData.Coord.x, y_dif = objData.Coord.y - pacman->objData.Coord.y;
	//	printf("%d %d\n",pacman->objData.Coord.x,pacman->objData.Coord.y);
	if (abs(x_dif) > abs(y_dif))
	{
		if (x_dif > 0)
		{
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::LEFT)
				{
					NextMove(Directions::LEFT);
					return;
				}
		}
		else
		{
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::RIGHT)
				{
					NextMove(Directions::RIGHT);
					return;
				}
		}
	}
	else
	{
		if (y_dif > 0)
		{
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::UP)
				{
					NextMove(Directions::UP);
					return;
				}
		}
		else
		{
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::DOWN)
				{
					NextMove(Directions::DOWN);
					return;
				}
		}
	}
	if (x_dif > 0)
	{
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::LEFT)
			{
				NextMove(Directions::LEFT);
				return;
			}
	}
	else
	{
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::RIGHT)
			{
				NextMove(Directions::RIGHT);
				return;
			}
	}
	if (y_dif > 0)
	{
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::UP)
			{
				NextMove(Directions::UP);
				return;
			}
	}
	else
	{
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::DOWN)
			{
				NextMove(Directions::DOWN);
				return;
			}
	}
	NextMove(proba[rand() % cnt]);
	// [TODO] (Not in Hackathon)
	// Description:
	// For red(Blinky) we ask you to implement an random strategy ghost,
	// which means moving in random direction.
	// But your random strategy have to designed carefully so that ghost won't walk back and forth.
	// (The code here DO perform walking back and forth.)
}

void GhostRed::move_script_BLOCKED(Map *M)
{
	switch (objData.preMove)
	{
	case Directions::UP:
		if (objData.Coord.y == 10)
			NextMove(Directions::DOWN);
		else
			NextMove(Directions::UP);
		break;
	case Directions::DOWN:
		if (objData.Coord.y == 12)
			NextMove(Directions::UP);
		else
			NextMove(Directions::DOWN);
		break;
	default:
		NextMove(Directions::UP);
		break;
	}
}

void GhostRed::move_script(Map *M, Pacman *pacman)
{
	if (!movetime(speed))
		return;
	switch (status)
	{
	case GhostStatus::BLOCKED:
		move_script_BLOCKED(M);
		//			printf("NOTICE:%d\n",al_get_timer_count(game_tick_timer)-ghost->previous_timer_val);
		if(!previous_timer_val){
			if(al_get_timer_count(game_tick_timer) > 0)
				status = GhostStatus::GO_OUT;
		} 
		else
		{
			if (al_get_timer_count(game_tick_timer) - previous_timer_val > GO_OUT_TIME * 2)
			{
				status = GhostStatus::GO_OUT;
			}
		}
		break;
	case GhostStatus::FREEDOM:
		move_script_FREEDOM(M, pacman);
		break;
	case GhostStatus::GO_OUT:
		move_script_GO_OUT(M);
		break;
	case GhostStatus::GO_IN:
		move_script_GO_IN(M);
		if (M->map[objData.Coord.y][objData.Coord.x] == 'B')
		{
			status = GhostStatus::BLOCKED;
			speed = 2;
		}
		break;
	case GhostStatus::FLEE:
		move_script_FLEE(M, pacman);
		break;
	default:
		break;
	}

	if (movable(M, objData.nextTryMove, false))
	{
		objData.preMove = objData.nextTryMove;
		objData.nextTryMove = Directions::NONE;
	}
	else if (!movable(M, objData.preMove, false))
		return;
	//		if(typeFlag==Pinky)
	//			printf("COOD::%d %d\n",objData.Coord.x,objData.Coord.y);
	switch (objData.preMove)
	{
	case Directions::RIGHT:
		objData.Coord.x += 1;
		break;
	case Directions::LEFT:
		objData.Coord.x -= 1;
		break;
	case Directions::UP:
		objData.Coord.y -= 1;
		break;
	case Directions::DOWN:
		objData.Coord.y += 1;
		break;
	default:
		break;
	}
	objData.facing = objData.preMove;
	objData.moveCD = GAME_TICK_CD;
}

void GhostPink::move_script_FREEDOM(Map *M, Pacman *pacman)
{
	static Directions proba[4];
	int i, cnt = 0;
	for (i = 1; i <= 4; i++)
		if (movable(M, (Directions)i, 1) && inv((Directions)i) != objData.preMove)
			proba[cnt++] = (Directions)i;
	if (!cnt)
	{
		NextMove(inv(objData.preMove));
		return;
	}
	int new_x=pacman->objData.Coord.x,new_y=pacman->objData.Coord.y;
	for(int i=1;i<=4;i++){
		switch(pacman->objData.facing){
			case Directions::UP:
				if(!M->is_wall_block(new_x,new_y-1)&&!M->is_room_block(new_x,new_y-1))
					new_y--;
				break;
			case Directions::DOWN:
				if(!M->is_wall_block(new_x,new_y+1)&&!M->is_room_block(new_x,new_y+1))
					new_y++;
				break;
			case Directions::LEFT:
				if(!M->is_wall_block(new_x-1,new_y)&&!M->is_room_block(new_x-1,new_y))
					new_x--;
				break;
			case Directions::RIGHT:
				if(!M->is_wall_block(new_x+1,new_y)&&!M->is_room_block(new_x+1,new_y))
					new_x++;
				break;
		}
	}
	Directions dir=M->shortest_path_direc(objData.Coord.x,objData.Coord.y,new_x,new_y);
	if(movable(M,dir,1))
		NextMove(dir);
	else
		NextMove(proba[rand()%cnt]);
}

void GhostPink::move_script_BLOCKED(Map *M)
{
	switch (objData.preMove)
	{
	case Directions::UP:
		if (objData.Coord.y == 10)
			NextMove(Directions::DOWN);
		else
			NextMove(Directions::UP);
		break;
	case Directions::DOWN:
		if (objData.Coord.y == 12)
			NextMove(Directions::UP);
		else
			NextMove(Directions::DOWN);
		break;
	default:
		NextMove(Directions::UP);
		break;
	}
}

void GhostPink::move_script(Map *M, Pacman *pacman)
{
	if (!movetime(speed))
		return;
	switch (status)
	{
	case GhostStatus::BLOCKED:
		move_script_BLOCKED(M);
		if (!previous_timer_val){
			if(al_get_timer_count(game_tick_timer) > GO_OUT_TIME)
				status = GhostStatus::GO_OUT;
		}
		else{
			if (al_get_timer_count(game_tick_timer) - previous_timer_val > GO_OUT_TIME << 1)
			status = GhostStatus::GO_OUT;
		} 
		break;
	case GhostStatus::FREEDOM:
		move_script_FREEDOM(M, pacman);
		break;
	case GhostStatus::GO_OUT:
		move_script_GO_OUT(M);
		break;
	case GhostStatus::GO_IN:
		move_script_GO_IN(M);
		if (M->map[objData.Coord.y][objData.Coord.x] == 'B')
		{
			status = GhostStatus::BLOCKED;
			speed = 2;
		}
		break;
	case GhostStatus::FLEE:
		move_script_FLEE(M, pacman);
	}
	if (movable(M, objData.nextTryMove, false))
	{
		objData.preMove = objData.nextTryMove;
		objData.nextTryMove = Directions::NONE;
	}
	else if (!movable(M, objData.preMove, false))
		return;
	switch (objData.preMove)
	{
	case Directions::RIGHT:
		++objData.Coord.x;
		break;
	case Directions::LEFT:
		--objData.Coord.x;
		break;
	case Directions::UP:
		--objData.Coord.y;
		break;
	case Directions::DOWN:
		++objData.Coord.y;
	}
	objData.facing = objData.preMove;
	objData.moveCD = GAME_TICK_CD;
}

void GhostBlue::move_script_FREEDOM(Map *M, Pacman *pacman)
{
	static Directions proba[4];
	int i, cnt = 0;
	for (i = 1; i <= 4; i++)
		if (movable(M, (Directions)i, 1) && inv((Directions)i) != objData.preMove)
			proba[cnt++] = (Directions)i;
	if (!cnt)
	{
		NextMove(inv(objData.preMove));
		return;
	}
	int new_x=pacman->objData.Coord.x,new_y=pacman->objData.Coord.y;
	for(int i=1;i<=2;i++){
		switch(pacman->objData.facing){
			case Directions::UP:
				if(!M->is_wall_block(new_x,new_y-1)&&!M->is_room_block(new_x,new_y-1))
					new_y--;
				break;
			case Directions::DOWN:
				if(!M->is_wall_block(new_x,new_y+1)&&!M->is_room_block(new_x,new_y+1))
					new_y++;
				break;
			case Directions::LEFT:
				if(!M->is_wall_block(new_x-1,new_y)&&!M->is_room_block(new_x-1,new_y))
					new_x--;
				break;
			case Directions::RIGHT:
				if(!M->is_wall_block(new_x+1,new_y)&&!M->is_room_block(new_x+1,new_y))
					new_x++;
				break;
		}
	}
	Directions dir= M->shortest_path_direc(objData.Coord.x,objData.Coord.y,objData.Coord.x+2*(new_x-ghosts[0]->objData.Coord.x),objData.Coord.y+2*(new_y-ghosts[0]->objData.Coord.y));
	if(movable(M,dir,1))
		NextMove(dir);
	else
		NextMove(proba[rand()%cnt]);
}

void GhostBlue::move_script_BLOCKED(Map *M)
{
	switch (objData.preMove)
	{
	case Directions::UP:
		if (objData.Coord.y == 10)
			NextMove(Directions::DOWN);
		else
			NextMove(Directions::UP);
		break;
	case Directions::DOWN:
		if (objData.Coord.y == 12)
			NextMove(Directions::UP);
		else
			NextMove(Directions::DOWN);
		break;
	default:
		NextMove(Directions::UP);
		break;
	}
}

void GhostBlue::move_script(Map *M, Pacman *pacman)
{
	if (!movetime(speed))
		return;
	switch (status)
	{
	case GhostStatus::BLOCKED:
		move_script_BLOCKED(M);
		if (!previous_timer_val ){
			if(al_get_timer_count(game_tick_timer) > GO_OUT_TIME * 3)
				status = GhostStatus::GO_OUT;
		}	
		else{
			if (al_get_timer_count(game_tick_timer) - previous_timer_val > GO_OUT_TIME << 1)
				status = GhostStatus::GO_OUT;
		} 
		break;
	case GhostStatus::FREEDOM:
		move_script_FREEDOM(M, pacman);
		break;
	case GhostStatus::GO_OUT:
		move_script_GO_OUT(M);
		break;
	case GhostStatus::GO_IN:
		move_script_GO_IN(M);
		if (M->map[objData.Coord.y][objData.Coord.x] == 'B')
		{
			status = GhostStatus::BLOCKED;
			speed = 2;
		}
		break;
	case GhostStatus::FLEE:
		move_script_FLEE(M, pacman);
	}
	if (movable(M, objData.nextTryMove, false))
	{
		objData.preMove = objData.nextTryMove;
		objData.nextTryMove = Directions::NONE;
	}
	else if (!movable(M, objData.preMove, false))
		return;
	switch (objData.preMove)
	{
	case Directions::RIGHT:
		++objData.Coord.x;
		break;
	case Directions::LEFT:
		--objData.Coord.x;
		break;
	case Directions::UP:
		--objData.Coord.y;
		break;
	case Directions::DOWN:
		++objData.Coord.y;
	}
	objData.facing = objData.preMove;
	objData.moveCD = GAME_TICK_CD;
}

void GhostOrange::move_script_FREEDOM(Map *M, Pacman *pacman)
{
	static Directions proba[4];
	int i, cnt = 0;
	for (i = 1; i <= 4; i++)
		if (movable(M, (Directions)i, 1) && inv((Directions)i) != objData.preMove)
			proba[cnt++] = (Directions)i;
	if (!cnt)
	{
		NextMove(inv(objData.preMove));
		return;
	}
	int x_dif = objData.Coord.x - pacman->objData.Coord.x, y_dif = objData.Coord.y - pacman->objData.Coord.y;
	if (abs(x_dif) > abs(y_dif))
	{
		if (x_dif > 0)
		{
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::LEFT)
				{
					NextMove(Directions::LEFT);
					return;
				}
		}
		else
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::RIGHT)
				{
					NextMove(Directions::RIGHT);
					return;
				}
	}
	else
	{
		if (y_dif > 0)
		{
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::UP)
				{
					NextMove(Directions::UP);
					return;
				}
		}
		else
			for (i = 0; i < cnt; i++)
				if (proba[i] == Directions::DOWN)
				{
					NextMove(Directions::DOWN);
					return;
				}
	}
	if (x_dif > 0)
	{
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::LEFT)
			{
				NextMove(Directions::LEFT);
				return;
			}
	}
	else
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::RIGHT)
			{
				NextMove(Directions::RIGHT);
				return;
			}
	if (y_dif > 0)
	{
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::UP)
			{
				NextMove(Directions::UP);
				return;
			}
	}
	else
		for (i = 0; i < cnt; i++)
			if (proba[i] == Directions::DOWN)
			{
				NextMove(Directions::DOWN);
				return;
			}
	NextMove(proba[rand() % cnt]);
}

void GhostOrange::move_script_BLOCKED(Map *M)
{
	switch (objData.preMove)
	{
	case Directions::UP:
		if (objData.Coord.y == 10)
			NextMove(Directions::DOWN);
		else
			NextMove(Directions::UP);
		break;
	case Directions::DOWN:
		if (objData.Coord.y == 12)
			NextMove(Directions::UP);
		else
			NextMove(Directions::DOWN);
		break;
	default:
		NextMove(Directions::UP);
		break;
	}
}

void GhostOrange::move_script(Map *M, Pacman *pacman)
{
	if (!movetime(speed))
		return;
	switch (status)
	{
	case GhostStatus::BLOCKED:
		move_script_BLOCKED(M);
		if (!previous_timer_val ){
			if(al_get_timer_count(game_tick_timer) > GO_OUT_TIME * 5){
				status = GhostStatus::GO_OUT;
			}
		}
		else{
			if (al_get_timer_count(game_tick_timer) - previous_timer_val > GO_OUT_TIME << 1)
				status = GhostStatus::GO_OUT;
		} 
		break;
	case GhostStatus::FREEDOM:
		move_script_FREEDOM(M, pacman);
		break;
	case GhostStatus::GO_OUT:
		move_script_GO_OUT(M);
		break;
	case GhostStatus::GO_IN:
		move_script_GO_IN(M);
		if (M->map[objData.Coord.y][objData.Coord.x] == 'B')
		{
			status = GhostStatus::BLOCKED;
			speed = 2;
		}
		break;
	case GhostStatus::FLEE:
		move_script_FLEE(M, pacman);
	}
	if (movable(M, objData.nextTryMove, false))
	{
		objData.preMove = objData.nextTryMove;
		objData.nextTryMove = Directions::NONE;
	}
	else if (!movable(M, objData.preMove, false))
		return;
	switch (objData.preMove)
	{
	case Directions::RIGHT:
		++objData.Coord.x;
		break;
	case Directions::LEFT:
		--objData.Coord.x;
		break;
	case Directions::UP:
		--objData.Coord.y;
		break;
	case Directions::DOWN:
		++objData.Coord.y;
	}
	objData.facing = objData.preMove;
	objData.moveCD = GAME_TICK_CD;
}
