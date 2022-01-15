#ifndef SCENE_END_H
#define SCENE_END_H
#include "game.hpp"

class SceneEnd : public Scene
{
public:
    SceneEnd();
    ~SceneEnd();
	virtual void draw();
	virtual void on_key_down(int);
};

#endif
