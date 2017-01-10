#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include <stdlib.h>  
#include "SDL/include/SDL.h"

#define CAMERA_SPEED_WALK 3;
#define CAMERA_SPEED_RUN 5;
// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	speed = 2;

	//Offset Renderer
	renderWithOffset = AnimationRender();

	//Animation states:
	state = IDLE;
	attackState = BASIC_PUNCH;

	// idle animation (just the ship)
	idle.frames.push_back({ 36, 23, 26, 34 });
	idle.frames.push_back({ 69, 23, 26, 34 });
	idle.speed = 0.025f;
	// move upwards
	up.frames.push_back({ 598, 26, 26, 35 });
	up.frames.push_back({ 628, 26, 26, 35 });
	up.speed = 0.1f;
	// Move down
	down.frames.push_back({ 598, 26, 26, 35 });
	down.frames.push_back({ 628, 26, 26, 35 });
	down.speed = 0.1f;
	//Move forward
	forward.frames.push_back({ 437, 26, 29, 34 });
	forward.frames.push_back({ 467, 26, 26, 33 });
	forward.frames.push_back({ 500, 30, 34, 31 });
	forward.frames.push_back({ 536, 26, 22, 37 });
	forward.frames.push_back({ 561, 30, 34, 29 });
	forward.speed = 0.1f;

	// Jump 
	startJumpPosition = 0;
	jumping = false;
	goingUp = false;
	jump.frames.push_back({ 415,167,47,26 });
	jump.speed = 0.1f;
	jumpHeight = 40.0f;
	// Attack
	punching = false;

	offsetLeftAttackLeft = { {0,-2},{9,-2} }; 
	offsetRightAttackLeft = { {0,-2},{0,-2} }; 

	offsetLeftAttackRight = { { 0,-2 },{ 13,-2 } };
	offsetRightAttackRight = { { 0,-2 },{ 0,-2 } };

	rightPunch.frames.push_back({ 99,25,27,32 });
	rightPunch.frames.push_back({ 128,25,39,32 });
	rightPunch.speed = 0.1f;
	leftPunch.frames.push_back({ 99,25,27,32 });
	leftPunch.frames.push_back({ 168,26,35,32 });
	leftPunch.speed = 0.1f;
	// KickAttack
	offsetLeftKick = { { 12, 0 },{ 18,14 } };
	offsetRighKick = { { 0,0 },{ 0,14 } };
	kickAttack.frames.push_back({ 167,165,41,35 });
	kickAttack.frames.push_back({ 212,154,43,48 });
	kickAttack.speed = 0.1f;
	// Final punch
	offsetLeftFinalPunch = { {8,6},{ 8,4 },{ 23,-2 },{ 23,21 } }; //positivo sube ,negativo baja
	offsetRighFinalPunch = { {0,6},{ 0,4 },{ 0,-2 },{ 0,21 } };

	finalPunch.frames.push_back({208,17,35,42});
	finalPunch.frames.push_back({250,19,34,39});
	finalPunch.frames.push_back({285,24,52,32}); 
	finalPunch.frames.push_back({ 345, 3,45,56 });
	finalPunch.speed = 0.1f;

	// Receive heavy attack
	offsetLeftReceiveHeavyAttack = { {0,0},{0,- 13 } };
	offsetRighReceiveHeavyAttack = { { 0,0 },{ 0,-13 } };
	receiveHeavyAttack.frames.push_back({84,165,34,35});
	receiveHeavyAttack.frames.push_back({120,175,42,21});
	receiveHeavyAttack.speed = 0.1f;
	receiveHeavyAttack.loop = false;
	timeDown = Temporizer(1);
}

ModulePlayer::~ModulePlayer()
{
	
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("rtype/BattletoadSprites/rash.gif");

	destroyed = false;
	position.x = 150;
	position.y = 200;
	
	SDL_Rect collRec;
	collRec.x = 150;
	collRec.y = 120;
	collRec.h = 33;
	collRec.w = 20;

	collider = App->collision->AddCollider(collRec);
	collider->colliderType = PLAYER;
	collider->addObserver(this);
	collider->active = true;

	SDL_Rect attackRect;
	attackRect.x = 150;
	attackRect.y = 120;
	attackRect.h = 15;
	attackRect.w = 20;
	

	colliderAttack = App->collision->AddCollider(attackRect);
	colliderAttack->colliderType = PLAYER_HIT;
	colliderAttack->addObserver(this);
	colliderAttack->active = false;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (life <= 0 && state != RECEIVE_HEAVY_ATTACK) {
		timeDown.Start();
		state = RECEIVE_HEAVY_ATTACK;
	}
	switch (state)
	{
	case IDLE:
		Idle();
		break;
	case WALK:
		Walk();
		break;
	case JUMP:
		Jump();
		break;
	case RUN:
		Run();
		break;
	case ATTACK:
		Attack();
		break;
	case SUPER_ATTACK:
		SuperAttack();
		break;
	case KICK_ATTACK:
		KickAttack();
		break;
	case RECEIVE_HEAVY_ATTACK:
		ReceiveHeavyAttack();
		break;
	default:
		Idle();
		break;
	}
	//178
   
	LockAttackZone();

	App->renderer->camera.y = -position.y ; 
	SetCollidersPosition();
	return UPDATE_CONTINUE;
}

void ModulePlayer::onNotify(GameEvent event) 
{
	switch (event) {
		case WALL_COLLISION:

			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				position.x -= speed;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				position.y += speed;

			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				position.y -= speed;

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				position.x += speed;

			break;
	}
	
}

void ModulePlayer::Walk() 
{	
	speed = 2;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_M) == KEY_IDLE) 
	{
		state = IDLE;
		return;
	}
		
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
	{	
		state = WALK;
		position.y -= speed;

		if (current_animation != &up)
		{
			up.Reset();
			current_animation = &up;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		state = WALK;
		position.y += speed;
		if (current_animation != &down)
		{
			down.Reset();
			current_animation = &down;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{	
		
		state = WALK;
		position.x -= speed;
		flipHorinzontal = true;
		current_animation = &forward;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{	
		state = WALK;
		position.x += speed;
		flipHorinzontal = false;
		current_animation = &forward;
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		state = RUN;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		goingUp = true;
		startJumpPosition = position.y;
		state = JUMP;
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT) {
		punching = true;
		state = ATTACK;
		return;
	}

	App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),1.0f, flipHorinzontal);
}

void ModulePlayer::Idle() 
{
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_M) == KEY_IDLE&&
		App->input->GetKey(SDL_SCANCODE_N)==KEY_IDLE && App->input->GetKey(SDL_SCANCODE_B) == KEY_IDLE&&
		App->input->GetKey(SDL_SCANCODE_U)==KEY_IDLE)
	{
		state = IDLE;
		current_animation = &idle;
		
	}
	
	else if (App->input->GetKey(SDL_SCANCODE_B) == KEY_REPEAT) {
		state = SUPER_ATTACK;
	}
	
	else if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT) {
		state = KICK_ATTACK;
	}
	else if (App->input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT) {
		timeDown.Start();
		state = RECEIVE_HEAVY_ATTACK;
	}
	else {
		state = WALK;
	}
	App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),1.0f, flipHorinzontal);
}

void ModulePlayer::Run() 
{
	speed = 4;
	current_animation = &forward;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		current_animation = &down;
		position.y -= speed;
		App->renderer->camera.y = -position.y;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		current_animation = &down;
		position.y += speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		current_animation = &forward;
		position.x -= speed;
		flipHorinzontal = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		current_animation = &forward;
		position.x += speed;
		flipHorinzontal = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) 
	{
		state = WALK;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) 
	{
		goingUp = true;
		startJumpPosition = position.y;
		state = JUMP;
	}

	App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), 1.0f, flipHorinzontal);
}

void ModulePlayer :: Jump() 
{	
	current_animation = &jump;
	if (goingUp == true) 
	{
		position.y -= 2;
		if (position.y <= startJumpPosition - jumpHeight)
			goingUp = false;

	}
	else {
		position.y+= 2;
		if (startJumpPosition <= position.y)
		{
			state = IDLE;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		flipHorinzontal = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		flipHorinzontal = false;
	}

	App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),1.0f, flipHorinzontal);
}

void ModulePlayer::Attack() 
{
	
	if (punching == true && current_animation->Finished()) {
		if (punchCounter%2== 0) {
			current_animation = &rightPunch;
		}
		else {
			current_animation = &leftPunch;
		}
	}

	if(current_animation == &rightPunch)
		renderWithOffset.Update(App, graphics, current_animation, flipHorinzontal, position, offsetLeftAttackRight, offsetRightAttackRight);
	else
		renderWithOffset.Update(App, graphics, current_animation, flipHorinzontal, position, offsetLeftAttackLeft, offsetRightAttackLeft);

	
	if (current_animation->Finished()) {
			state = IDLE;
			punching = false;
			rightPunch.Reset();
			leftPunch.Reset();
			++punchCounter;
			return;
		
	}
}


void ModulePlayer::SuperAttack() {
	current_animation = &finalPunch;
	offsetLeftFinalPunch; 
	renderWithOffset.Update(App, graphics, current_animation, flipHorinzontal, position, offsetLeftFinalPunch, offsetRighFinalPunch);
	if (current_animation->Finished()) {
		finalPunch.Reset();
		state = IDLE;
	}
}


void ModulePlayer::KickAttack() {

	current_animation = &kickAttack;
	renderWithOffset.Update(App,graphics,current_animation,flipHorinzontal,position, offsetLeftKick, offsetRighKick);
	
	if (current_animation->Finished()) {
		kickAttack.Reset();
		state = IDLE;
	}
}

void ModulePlayer::ReceiveHeavyAttack() {
	current_animation = &receiveHeavyAttack;
	
	renderWithOffset.Update(App, graphics, current_animation, flipHorinzontal, position, offsetLeftReceiveHeavyAttack, offsetRighReceiveHeavyAttack);
	
	if (current_animation->Finished() && timeDown.Update()) {
		receiveHeavyAttack.Reset();
		state = IDLE;
		timeDown.Reset();
		life = 2000;
	}

}

void ModulePlayer::SetCollidersPosition() {
	if (flipHorinzontal) {
		collider->SetPos(position.x + 3, position.y);
		colliderAttack->SetPos(position.x - 15, position.y);
	}
	else {
		collider->SetPos(position.x + 5, position.y);
		colliderAttack->SetPos(position.x + 20, position.y);
	}
}

void ModulePlayer::LockAttackZone() {
	bool enemiesActive= App->enemies->EnemiesAlive();
	
	if (enemiesActive == true && flagLockAttackZone ==false)
	{
		blockRight = position.x + 190;
		blockLeft = position.x - 180;
		initialLockPosition = blockRight;
		flagLockAttackZone = true;
	}

	if (enemiesActive == true) {
		
		if (position.x >= blockRight)
			position.x -= speed;
		if (position.x <= blockLeft)
			position.x += speed;
	
	}
	else {
		flagLockAttackZone = false;
		if (position.x > 178) 
		{
			if (App->renderer->camera.x > -(position.x * SCREEN_SIZE - 500)) 
			{
				App->renderer->camera.x -= speed*3;

				if (App->renderer->camera.x >= -(position.x * SCREEN_SIZE - 500)) {
					App->renderer->camera.x = -(position.x * SCREEN_SIZE - 500);
					return;
				}
				
			}

			else if (App->renderer->camera.x < -(position.x * SCREEN_SIZE - 500))
			{
				App->renderer->camera.x += speed * 3;

				if (App->renderer->camera.x <= -(position.x * SCREEN_SIZE - 500)) 
				{
					App->renderer->camera.x = -(position.x * SCREEN_SIZE - 500);
					return;
				}
			}
		}
	}
	
}

//App->renderer->camera.x = -(position.x * SCREEN_SIZE - 500);