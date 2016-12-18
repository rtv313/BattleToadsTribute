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
	/*beforePunch.frames.push_back({99,25,27,32});
	beforePunch.speed = 0.1f;*/
	punching = false;
	rightPunch.frames.push_back({ 99,25,27,32 });
	rightPunch.frames.push_back({128,25,39,32});
	rightPunch.loop = false;
	rightPunch.speed = 0.05f;
	leftPunch.frames.push_back({ 99,25,27,32 });
	leftPunch.frames.push_back({168,26,35,32});
	leftPunch.loop = false;
	leftPunch.speed = 0.05f;
	finalPunch.frames.push_back({ 209,17,35,41});
	finalPunch.frames.push_back({ 250,19,34,39});
	finalPunch.frames.push_back({ 285,24,52,32});
	finalPunch.frames.push_back({ 345, 2,45,56});
	finalPunch.speed = 0.1f;
	punchTemporizer=(0.5);
	runTemporizer = (1.5);
	
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
		break;
	case RUN:
		Run();
		break;
	case ATTACK:
		break;
	default:
		break;
	}
	


	

	// Draw everything --------------------------------------
	if(destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),1.5f,flipHorinzontal);

	Jump();

	return UPDATE_CONTINUE;
}







void ModulePlayer::onNotify(GameEvent event) 
{
	
}

void ModulePlayer::Jump() 
{
	if (jumping == true) {
		if (goingUp == true) {
			position.y -= speed;
			if (position.y <= startJumpPosition - jumpHeight) 
				goingUp = false;
			
		}else{
			position.y += speed;
			if (startJumpPosition  <= position.y)
				jumping = false;
			}
	}
	else {
		jumpHeight = 35;
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
	}
	else {
		state = WALK;
	}
	
}

void ModulePlayer::Run() 
{
	speed = 4;
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		current_animation = &forward;
		flipHorinzontal = true;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		current_animation = &forward;
		flipHorinzontal = false;
		
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) {
		state = WALK;
		
	}


	//state = IDLE;
	
	
}