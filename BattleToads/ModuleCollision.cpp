#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

using namespace std;

ModuleCollision::ModuleCollision()
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
		for (list<Collider*>::iterator b = colliders.begin(); b != colliders.end();++b)
		{
			if (indexA != indexB && (*b)->CheckCollision((*a)->rect)) { // if collide what we do ?
				(*a)->ValidCollision((*b));
			}
			++indexB;
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
		App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 80);
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
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
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
	
 	if (CollisionMatrix[colliderType][collider->colliderType]) {
		if(colliderType!=WALL)
			to_delete = true;
		for (int i = 0; i < numObservers_; i++) {
			observers_[0]->onNotify(DESTROY_PARTICLE);
		}
	}
}

void Collider::addObserver(Observer* observer)
{
	observers_[numObservers_] = observer;
	++numObservers_;
}

void Collider::removeObserver(Observer* observer) 
{
	for (int i = 0; i < numObservers_; i++) {
		observers_[i] = nullptr;
	}
}