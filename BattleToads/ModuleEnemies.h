
#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Globals.h"
#include "Point.h"
#include "Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "AnimationRender.h"

class ModuleEnemies :
	public Module
{
public:
	SDL_Rect rect = { 0,0,0,0 };
	bool to_delete = false;
	ModuleEnemies();
	~ModuleEnemies();
};

#endif