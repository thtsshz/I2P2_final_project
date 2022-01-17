#ifndef GHOST_H
#define GHOST_H
#include "utility.h"
#include "game.hpp" 
typedef struct Pacman Pacman;
typedef struct Map Map;

typedef enum class GhostStatus{
	BLOCKED,						 // stay inside the ghost room
	GO_OUT,							 // going out the ghost room
	FREEDOM,					     // free at the map
	GO_IN,							 // going back to the ghost room 
	FLEE							 // pacman powered up
} GhostStatus;

class Ghost
{
private:
	bitmapdata imgdata;
	ALLEGRO_BITMAP* flee_sprite;
	ALLEGRO_BITMAP* dead_sprite;
protected:
	ALLEGRO_BITMAP* move_sprite;
public:
	object objData;
	int speed;
	int64_t previous_timer_val;
	GhostStatus status;
	Ghost();
	~Ghost();
	void draw();
	void move(Map *);
	void NextMove(Directions);
	void printGhostStatus(GhostStatus);
	bool movable(Map *, Directions, bool);
	void toggle_FLEE(bool);
	void collided();
	void move_script_GO_IN(Map *);
	void move_script_GO_OUT(Map *);
	void move_script_FLEE(Map *, const Pacman * const);
	virtual void move_script(Map *, Pacman *) {}
	virtual void move_script_FREEDOM(Map *, Pacman *) {}
	virtual void move_script_BLOCKED(Map *) {}
};

class GhostRed : public Ghost
{
public:
	GhostRed();
	void move_script(Map *, Pacman *);
	void move_script_FREEDOM(Map *, Pacman *);
	void move_script_BLOCKED(Map *);
};

class GhostPink : public Ghost
{
public:
	GhostPink();
	void move_script(Map *, Pacman *);
	void move_script_FREEDOM(Map *, Pacman *);
	void move_script_BLOCKED(Map *);
};

class GhostBlue : public Ghost
{
public:
	GhostBlue();
	void move_script(Map *, Pacman *);
	void move_script_FREEDOM(Map *, Pacman *);
	void move_script_BLOCKED(Map *);
};

class GhostOrange : public Ghost
{
public:
	GhostOrange();
	void move_script(Map *, Pacman *);
	void move_script_FREEDOM(Map *, Pacman *);
	void move_script_BLOCKED(Map *);
};

#endif
