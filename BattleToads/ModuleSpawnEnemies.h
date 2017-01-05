#ifndef __ModuleSpawnEnemies_H__
#define __ModuleSpawnEnemies_H__
#include<list>
#include "Module.h"


class SpawnZone 
{	public:
	bool to_delete = false;
	SDL_Rect rect = { 0,0,0,0 };
	bool CheckCollision(const SDL_Rect& r) const;
	
};

class ModuleSpawnEnemies :
	public Module
{
public:
	std::list<SpawnZone*> spawnZones;
	ModuleSpawnEnemies();
	~ModuleSpawnEnemies();
	update_status PreUpdate();
	update_status Update();
	SpawnZone* AddSpawnZone();
	bool CleanUp();
	void DebugDraw();
};
#endif