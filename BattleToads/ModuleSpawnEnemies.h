#ifndef __ModuleSpawnEnemies_H__
#define __ModuleSpawnEnemies_H__

#include<list>
#include "Globals.h"
#include "Module.h"

class SpawnZone
{
public:
	bool to_delete = false;
	bool activate = false;
	SDL_Rect rect = { 0,0,0,0 };
	SpawnZone(const SDL_Rect& rect) :rect(rect) {};
	//add animation
};

class SpawnTrigger 
{	
	public:
		std::list<SpawnZone*> spawnZones;
		bool to_delete = false;
		bool active = false;
		SDL_Rect rect = { 0,0,0,0 };
		SpawnTrigger(const SDL_Rect& rectangle);
		bool CheckCollision(const SDL_Rect& r) const;
		void CreateEnemies();
		//add method for add spawnzones
};

class ModuleSpawnTriggers : public Module
{
public:
	bool debug = true;
	std::list<SpawnTrigger*> spawnTriggers;
	ModuleSpawnTriggers();
	~ModuleSpawnTriggers();
	update_status PreUpdate();
	update_status Update();
	SpawnTrigger* AddSpawnTrigger(const SDL_Rect& rect);
	bool CleanUp();
	void DebugDraw();
};

class ModuleSpawnZones : public Module 
{	
public:
	bool debug = false;
	std::list<SpawnZone*> spawnZones;
	ModuleSpawnZones();
	~ModuleSpawnZones();
	update_status PreUpdate();
	update_status Update();
	SpawnZone* AddSpawnZone(const SDL_Rect& rect);
	bool CleanUp();
	void DebugDraw();
	//add graphics and animations
};
#endif