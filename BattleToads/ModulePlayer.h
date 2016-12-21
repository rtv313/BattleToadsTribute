#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "Observer.h"
#include "ModuleCollision.h"
#include "Temporizer.h"
#include "AnimationRender.h"
struct SDL_Texture;

enum AnimationState { IDLE,WALK,JUMP,RUN,ATTACK,SUPER_ATTACK,KICK_ATTACK }; // Desirable have dragon , stick
enum AnimationAttacksState { BASIC_PUNCH,SUPER_PUNCH,HEAD_PUNCH, KICK_PUNCH, TONGUE_PUNCH };

class ModulePlayer : public Module ,Observer
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void onNotify(GameEvent event);

private:
	void Jump();
	void Walk();
	void Idle();
	void Run();
	void Attack();
	void SuperAttack();
	void KickAttack();
	
public:
	int speed;
	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	iPoint position;
	bool destroyed = false;
	Collider *collider;
	
	// Walk
	Animation forward;
	Animation backward;
	Animation up;
	Animation down;
	//idle
	Animation idle;
	// jump
	Animation jump;
	bool jumping;
	bool goingUp;
	int startJumpPosition;
	int jumpHeight;
	//Attack
	Animation rightPunch;
	Animation leftPunch;
	vector<iPoint> offsetLeftAttackLeft;
	vector<iPoint> offsetRightAttackLeft;
	vector<iPoint> offsetLeftAttackRight;
	vector<iPoint> offsetRightAttackRight;
	bool punching;
	//Kick Attack
	vector<iPoint> offsetLeftKick;
	vector<iPoint> offsetRighKick;
	AnimationRender renderWithOffset;
	Animation kickAttack;
	//Super Attack
	Animation finalPunch;

private:


	int punchCounter = 0;
	bool flipHorinzontal = false;
	AnimationState state;
	AnimationAttacksState attackState;


};

#endif