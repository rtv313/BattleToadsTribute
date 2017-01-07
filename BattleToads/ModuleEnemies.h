
#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Globals.h"
#include "Point.h"
#include "Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "AnimationRender.h"
#include "ModuleCollision.h"

class Enemy 
{
public:
	int life;
	int attack;
	SDL_Rect rect = { 0,0,0,0 };
	void Update();
	Enemy();
	~Enemy();
private:
	void Walk();
	void Run();
	void Attack();
	void UnderAttack();
	void Die();
};

class ModuleEnemies :
	public Module
{
public:
	
	bool to_delete = false;
	ModuleEnemies();
	~ModuleEnemies();
	update_status PreUpdate();
	update_status Update();
	Enemy* AddSpawnEnemy(const int  x, const int  y, const int  h, const int  w);
	bool CleanUp();
	void DebugDraw();
	bool Start();

};

#endif