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
#include "SDL/include/SDL.h"


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	
	// idle animation (just the ship)
	idle.frames.push_back({ 36, 23, 26, 34 });
	idle.frames.push_back({ 69, 23, 26, 34 });
	idle.speed = 0.025f;
	// move upwards
	up.frames.push_back({598, 26, 26, 35});
	up.frames.push_back({628, 26, 26, 35});
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
	forward.speed = 0.2f;

	// Jump 
	startJumpTime = 0;
	jumping = false;
	goingUp = false;
	jump.frames.push_back({415,167,47,26});
	jump.speed = 0.1f;
	secondsTarget = 300.0f;

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
	int speed = 2;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		flipHorinzontal = true;
		current_animation = &forward;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		flipHorinzontal = false;
		current_animation = &forward;
		
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y += speed;
		if(current_animation != &down)
		{
			down.Reset();
			current_animation = &down;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y -= speed;
		if(current_animation != &up)
		{
			up.Reset();
			current_animation = &up;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && ((App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) {
		current_animation = &forward;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && (( App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) ||  App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) {
		current_animation = &forward;
	}


	collider->SetPos(position.x,position.y); //update collider position

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
		App->particles->AddParticle(App->particles->laser , App->player->position.x + 20, App->player->position.y);
		current_animation = &jump;
		jumping = true;
		goingUp = true;
		startJumpTime = SDL_GetTicks();
	}
	Jump(speed);


	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_A)==KEY_IDLE && App->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
		current_animation = &idle;

	// Draw everything --------------------------------------
	if(destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()),1.5f,flipHorinzontal);

	return UPDATE_CONTINUE;
}

// TODO 13: Make so is the laser collides, it is removed and create an explosion particle at its position

// TODO 14: Make so if the player collides, it is removed and create few explosions at its positions
// then fade away back to the first screen (use the "destroyed" bool already created 
// You will need to create, update and destroy the collider with the player

void ModulePlayer::onNotify(GameEvent event) {
	
}

void ModulePlayer::Jump(int &speed) {
	if (jumping == true) {
		if (goingUp == true){
			position.y -= speed;
			if ((SDL_GetTicks() - startJumpTime) >= secondsTarget) {
				goingUp = false;
				startJumpTime = SDL_GetTicks();
			}
		}
		else if(goingUp == false){
			position.y += speed;
			if ((SDL_GetTicks() - startJumpTime) >= secondsTarget) {
				jumping = false;
			}
		}
	}
}
