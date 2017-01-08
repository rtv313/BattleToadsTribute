#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleEnemies.h"

using namespace std;

ModuleCollision::ModuleCollision()
{

}

ModuleCollision::ModuleCollision(bool active):Module(active)
{

}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	// TODO 8: Check collisions between all colliders. 
	// After making it work, review that you are doing the minumum checks possible 
	int indexA = 0;
	int indexB = 0;
	for (list<Collider*>::iterator a = colliders.begin(); a != colliders.end(); ++a)
	{
		bool collisionWall = false;
		for (list<Collider*>::iterator b = colliders.begin(); b != colliders.end();++b)
		{
			if (indexA != indexB && (*a)->CheckCollision((*b)->rect)) { // if collide what we do ?
				(*b)->ValidCollision((*a));
				if ((*a)->colliderType == SENSOR && (*b)->colliderType == WALL) {
					collisionWall = true;
				}
				else if((*a)->colliderType== SENSOR && (*b)->colliderType == PLAYER) {
					collisionWall = true;
				}
			}
			
			++indexB;
		} 

		if (collisionWall == false && (*a)->colliderType == SENSOR)
		{
			(*a)->NoCollision();
		}
		++indexA;
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		switch ((*it)->colliderType)
		{
		
		case ENEMY:
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
			break;
		case SENSOR:
			App->renderer->DrawQuad((*it)->rect, 255, 204, 51, 80);
			break;
		default:
			App->renderer->DrawQuad((*it)->rect, 0, 255, 0, 80);
			break;
		}
		
	}
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect)
{
	Collider* ret = new Collider(rect);

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// Return true if the argument and the own rectangle are intersecting
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = rect.x;
	rightA = rect.x + rect.w;
	topA = rect.y;
	bottomA = rect.y + rect.h;

	leftB = r.x;
	rightB = r.x + r.w;
	topB = r.y;
	bottomB = r.y + r.h;

	if (bottomA <= topB) { 
		return false; 
	} 

	if (topA >= bottomB) {
		return false; 
	} 

	if (rightA <= leftB) {
		return false; 
	}

	if (leftA >= rightB) {
		return false; 
	}

	return true;
}


void Collider::ValidCollision(Collider * collider) {
	
		
		for (list<Observer*>::iterator observer = observers_.begin(); observer != observers_.end(); ++observer)
		{
			if (colliderType == PLAYER && collider->colliderType == WALL)
			{
				(*observer)->onNotify(WALL_COLLISION);
			}

			if (colliderType == SENSOR && collider->colliderType == WALL) 
			{
				(*observer)->onNotify(WALL_COLLISION,collider->rect.y + collider->rect.h);
			}

			if (colliderType == SENSOR && collider->colliderType == PLAYER) {
				(*observer)->onNotify(PLAYER_COLLISION);
			}
		}

	
 }


void Collider::addObserver(Observer* observer)
{
	observers_.push_back(observer);
	
}

void Collider::removeObserver(Observer* observer) 
{
	for (list<Observer*>::iterator it = observers_.begin(); it != observers_.end(); ++it) {

		if ((*it) == observer)
		{
			RELEASE(*it);
			it = observers_.erase(it);
			return;
		}
	}
}

void Collider::NoCollision()
{
	for (list<Observer*>::iterator it = observers_.begin(); it != observers_.end(); ++it) 
	{
		(*it)->onNotify(NO_COLLISION);
	}
}