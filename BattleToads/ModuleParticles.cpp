#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "SDL/include/SDL_timer.h"
#include "ModulePlayer.h"


ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("Assets/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	laser =  Particle();
	laser.sprite.frames.push_back({ 232, 103, 16, 12 });
	laser.sprite.frames.push_back({ 249, 103, 16, 12 });
	laser.sprite.speed = 0.08f;
	laser.audioID = App->audio->LoadFx("Assets/laser.wav");
	laser.ttl = 10;
	laser.speed = 6;
	laser.Type = PROJECTILE;
	laser.posX = 0;
	laser.posY = 0;
	laser.frame = 0;

	SDL_Rect collRec;
	collRec.x = 1;
	collRec.y = 1;
	collRec.h = 16;
	collRec.w = 16;
	laser.collider = App->collision->AddCollider(collRec);
	laser.collider->colliderType = PLAYER_LASER;
	laser.optionalCollider = App->collision->AddCollider(collRec);
	laser.optionalCollider->colliderType = PLAYER_LASER;

	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	explosion = Particle();
	explosion.ttl = 1;
	explosion.speed = 1;
	explosion.Type = EXPLOSION;
	explosion.posX = 0;
	explosion.posY = 0;
	explosion.frame = 0;
	explosion.audioID= App->audio->LoadFx("Assets/explosion.wav");
	explosion.sprite.frames.push_back({ 274, 296, 33, 30 });
	explosion.sprite.frames.push_back({ 313, 296, 33, 30 });
	explosion.sprite.frames.push_back({ 346, 296, 33, 30 });
	explosion.sprite.frames.push_back({ 382, 296, 33, 30 });
	explosion.sprite.frames.push_back({ 419, 296, 33, 30 });
	explosion.sprite.frames.push_back({ 457, 296, 33, 30 });
	explosion.sprite.speed = 0.08f;

	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{	
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		Particle* p = *it;
		
		if(p->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else {
			App->renderer->Blit(graphics, p->posX, p->posY, &(p->sprite.GetCurrentFrame()), 1.0f);
			++it;
			
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle	
	Particle * newParticle = new Particle(particle);
	newParticle->posX = x;
	newParticle->posY = y;
	active.push_back(newParticle);
	App->audio->PlayFx(newParticle->audioID);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{
	
}

// TODO 3: Fill in a copy constructor
Particle::Particle(const Particle& p) 
{
	posX = p.posX;
	posY = p.posY;
	speed = p.speed;
	Type = p.Type;
	ttl = p.ttl;
	audioID = p.audioID;
	sprite = p.sprite;
	frame = p.frame;
	SDL_Rect collRec;
	collRec.x = 1;
	collRec.y = 1;
	collRec.h = 16;
	collRec.w = 16;
	if (p.Type != EXPLOSION) {

		collider = App->collision->AddCollider(collRec);
		collider->colliderType = p.collider->colliderType;
		collider->addObserver(this);

		optionalCollider = App->collision->AddCollider(collRec);
		optionalCollider->colliderType = p.optionalCollider->colliderType;
		optionalCollider->addObserver(this);
	}
 }

Particle::~Particle()
{
}

bool Particle::Update()
{
	bool ret = true;
	
	if (Type != EXPLOSION) {
		collider->SetPos(posX, posY);
		posX += speed;
	}
	// TODO 5: This is the core of the particle functionality.
	// Return false if the particle must be destroyed
	if (frame <= 30) {
		++frame;
	}
	else {
		--ttl;
		frame = 0;
	}

	if (ttl <= 0) { 
		ret = false; 
		if (Type != EXPLOSION)
			collider->to_delete = true;
	}
	
	return ret;
}

void Particle::onNotify(GameEvent event) {
	if (event == DESTROY_PARTICLE) {
		ttl = 0;
		App->particles->AddParticle(App->particles->explosion,posX, posY); //create explosion
	}
}