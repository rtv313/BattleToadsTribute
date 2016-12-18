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
#include <stdlib.h>  
#include "SDL/include/SDL.h"


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{	
	speed = 2;

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
	// Punch
	punching = false;
	rightPunch.frames.push_back({ 99,25,27,32 });
	rightPunch.frames.push_back({128,25,39,32});
	rightPunch.speed = 0.1f;
	leftPunch.frames.push_back({ 99,25,27,32 });
	leftPunch.frames.push_back({ 168,26,35,32 });
	leftPunch.speed = 0.1f;
	
	punchTemporizer=(0.25);

	
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("rtype/BattletoadSprites/rash.gif");

	destroyed = false;
	position.x = 150;
	position.y = 120;

	SDL_Rect collRec;
	collRec.x = 150;
	collRec.y = 120;
	collRec.h = 16;
	collRec.w = 16;
	collider = App->collision->AddCollider(collRec);
	collider->colliderType = PLAYER;
	collider->addObserver(this);
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
	default:
		break;
	}
	


	

	// Draw everything --------------------------------------
	if(destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),0.1f,flipHorinzontal);


	return UPDATE_CONTINUE;
}

void ModulePlayer::onNotify(GameEvent event) 
{
	
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
	}
}

void ModulePlayer::Idle() 
{
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE &&
		App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_M) == KEY_IDLE)
	{
		state = IDLE;
		current_animation = &idle;
		return;
	}else if(App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT) {
		state = ATTACK;
	}
	else{
		state = WALK;
	}
	
}

void ModulePlayer::Run() 
{
	speed = 4;
	current_animation = &forward;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
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
		position.y += 2;
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
}

void ModulePlayer::Attack() 
{
	state = ATTACK;
	if (punching == true && current_animation->Finished()) {
		int  randomPunch = rand() % 3 + 1;
		
		if (randomPunch % 2 == 0)
			current_animation = &rightPunch;
		else
			current_animation = &leftPunch;
		
	}
	if (current_animation->Finished()) {
		
			state = IDLE;
			punching = false;
			rightPunch.Reset();
			leftPunch.Reset();
		
	}
	
}