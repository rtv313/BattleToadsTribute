
#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Globals.h"
#include<list>
#include "Point.h"
#include "Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "AnimationRender.h"
#include "ModuleCollision.h"
enum EnemyState { WALK_ENEMY, ATTACK_ENEMY, UNDER_ATTACK, DIE };

class Enemy 
{
public:
	bool to_delete;
	int life = 100;
	int attack = 10;
	bool flipHorizontal = false;
	Animation animation;
	SDL_Rect rect = { 0,0,0,0 };
	
	Collider * body;
	Collider * sensorLeft;
	Collider * sensorRight;
	void Update();
	Enemy();
	Enemy(int x, int y);
	~Enemy();
private:
	void Walk();
	void Attack();
	void UnderAttack();
	void Die();
	void CheckLife();
	void UpdateCollidersPosition();
};

class ModuleEnemies :
	public Module
{
public:
	bool debug = true;
	list<Enemy *> enemies;
	SDL_Texture* graphics;
	AnimationRender animationRender;
	ModuleEnemies(bool active);
	~ModuleEnemies();
	update_status PreUpdate();
	update_status Update();
	Enemy* AddEnemy(int x , int y);
	bool CleanUp();
	void DebugDraw();
	bool Start();

};

#endif