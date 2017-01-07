
#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__


#include<list>
#include "Globals.h"
#include "Observer.h"
#include "Point.h"
#include "Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "AnimationRender.h"
#include "ModuleCollision.h"
enum EnemyState { WALK_ENEMY, ATTACK_ENEMY, UNDER_ATTACK, DIE };

class Enemy :Observer
{
public:
	bool to_delete;
	int life = 100;
	int attack = 10;
	int speed = 1;
	bool flipHorizontal = false;
	iPoint position;
	Animation animation;
	
	
	Collider * body;
	Collider * sensorLeft;
	Collider * sensorRight;
	void Update();
	void onNotify(GameEvent event);
	Enemy();
	Enemy(int x, int y);
	~Enemy();
private:
	EnemyState state;
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