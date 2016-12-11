#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"
#include "Observer.h"





struct SDL_Texture;

enum ParticleType { PROJECTILE,EXPLOSION};


class Particle:public Observer
{	
public:
	//bool to_delete = false;
	// TODO 1: Fill in the structure with all the properties you need for each particle
	int posX, posY;
	int speed;
	ParticleType Type;
	float ttl;
	int audioID;
	Animation sprite;
	int frame;
	Collider* collider;
	// TODO 11: Add an optional collider to each particle
	Collider* optionalCollider;
	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
	void onNotify(GameEvent event);
};




class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddParticle(const Particle& particle, int x, int y); // feel free to expand this call

private:

	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:

	// prototype particles go here ...
	Particle laser;
	Particle explosion;
};

#endif // __MODULEPARTICLES_H__