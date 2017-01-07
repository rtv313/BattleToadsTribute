#ifndef __ModuleSpawnEnemies_H__
#define __ModuleSpawnEnemies_H__

#include<list>
#include "Globals.h"
#include "Point.h"
#include "Animation.h"
#include "Module.h"
#include "ModuleTextures.h"
#include "AnimationRender.h"
class SpawnZone
{
public:
	bool to_delete = false;
	bool activate = false;
	bool createdEnemy = false;
	SDL_Rect rect = { 0,0,0,0 };
	Animation  spawnAnimationOne;
	SpawnZone(const SDL_Rect& rect) :rect(rect)
	{
		spawnAnimationOne.frames.push_back({ 19,160, 20, 9 }); 
		spawnAnimationOne.frames.push_back({ 39,158,  24, 12 });
		spawnAnimationOne.frames.push_back({ 120,152, 28, 18 });
		spawnAnimationOne.frames.push_back({ 180,148, 28, 22 });
		spawnAnimationOne.frames.push_back({ 180,148, 28, 24 });
		spawnAnimationOne.frames.push_back({ 1,194, 28, 30 });
		spawnAnimationOne.frames.push_back({ 59,190, 28, 34 });
		spawnAnimationOne.frames.push_back({ 117,186, 28,38 });
		spawnAnimationOne.frames.push_back({ 175,182, 28,42 });
		spawnAnimationOne.frames.push_back({ 233,178, 28,46 });
		spawnAnimationOne.frames.push_back({ 291,174, 28,50 });

		// down 
		spawnAnimationOne.frames.push_back({ 291,174, 28,50 });
		spawnAnimationOne.frames.push_back({ 233,178, 28,46 });
		spawnAnimationOne.frames.push_back({ 175,182, 28,42 });
		spawnAnimationOne.frames.push_back({ 117,186, 28,38 });
		spawnAnimationOne.frames.push_back({ 59,190, 28, 34 });
		spawnAnimationOne.frames.push_back({ 1,194, 28, 30 });
		spawnAnimationOne.frames.push_back({ 180,148, 28, 24 });
		spawnAnimationOne.frames.push_back({ 180,148, 28, 22 });
		spawnAnimationOne.frames.push_back({ 120,152, 28, 18 });
		spawnAnimationOne.frames.push_back({ 39,158,  24, 12 });
		spawnAnimationOne.frames.push_back({ 19,160, 20, 9 });

		spawnAnimationOne.loop = false;
		spawnAnimationOne.speed = 0.3;
	
	};
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
		void AddSpawnZone(const int  x, const int  y, const int  h, const int  w);
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
	SpawnTrigger* AddSpawnTrigger(const int  x, const int  y, const int  h, const int  w);
	bool CleanUp();
	void DebugDraw();
};

class ModuleSpawnZones : public Module 
{	
public:
	bool debug = false;
	SDL_Texture* spawnSprite = nullptr;
	vector<iPoint> offsetSpawnOneLeft;
	AnimationRender renderWithOffset;
	std::list<SpawnZone*> spawnZones;
	ModuleSpawnZones(bool active);
	~ModuleSpawnZones();
	update_status PreUpdate();
	update_status Update();
	SpawnZone* AddSpawnZone(const int  x, const int  y, const int  h, const int  w);
	bool CleanUp();
	void DebugDraw();
	bool Start();
	//add graphics and animations
};
#endif