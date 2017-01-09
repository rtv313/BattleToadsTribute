
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
enum EnemyState { WALK_ENEMY, ATTACK_ENEMY, UNDER_ATTACK, DIE_ENEMY ,ENEMY_IDLE,MOVE_OTHER_SIDE};

class Enemy :Observer
{
public:
	bool to_delete;
	int life = 100;
	int attack = 10;
	bool go_down = false;
	int go_down_offset = 0;
	int speed = 1;
	bool flipHorizontal = false;
	iPoint position;
	Animation* currentAnimation;
	Animation animationWalk;
	Animation animationAttack;
	Animation animationDead;
	int wallPositionTarget = 0;
	int targetPositionAttack = 0;
	int tiempoatacando = 0; // for testing
	int tiempoIdle = 0;
	Collider * body;
	Collider * sensor;

	void Update();
	void onNotify(GameEvent event);
	void onNotify(GameEvent event, int position);
	Enemy();
	Enemy(int x, int y);
	~Enemy();
	EnemyState state;
private:
	bool  PlayerInYourDirection();
	void Walk();
	void Idle();
	void MoveOtherSide();
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