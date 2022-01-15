#ifndef SCENE_MENU_H
#define SCENE_MENU_H
#include "game.hpp"
class SceneMenu : public Scene
{
public:
    SceneMenu();
    ~SceneMenu();
	virtual void draw();
	virtual void on_key_down(int);
	virtual void on_mouse_down(int, int, int, int);
	virtual void on_mouse_move(int, int, int, int);
};
#endif
