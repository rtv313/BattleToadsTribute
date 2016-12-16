#include "SDL/include/SDL.h" 

class Temporizer {

public:

	float  startTime;
	float  setTime;

	Temporizer():setTime(1000) {}
	Temporizer(float setTime) :setTime(setTime*1000) {}
	
	void Reset() {
		startTime = SDL_GetTicks();
	}

	bool Update() {
		if (SDL_GetTicks() - startTime >= setTime) {
			startTime = SDL_GetTicks();
			return true;
		}
		return false;
	}

};
