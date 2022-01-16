#ifndef GHOST_H
#define GHOST_H
#include "utility.h"
#include "game.hpp"
// <Forward Declaration>
// forward declarations can be useful when you need to have looping struct declarations,
// but you want to (should) prevent mutually include dependency.
// Only applicable when you only need the declaration but no need the definition in you sturct,
// which means that you only used the struct as pointer(no access to struct data), param, return type in this file.
// If you have to access the data in struct ex. `A->data`, 
// you should include the corresponding header file for its definition.
// Advance reading : https://stackoverflow.com/questions/18658438/what-is-forward-declaration-and-the-difference-between-typedef-struct-x-and

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
