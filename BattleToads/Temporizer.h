#include "SDL/include/SDL.h" 

class Temporizer {

public:

	float  startTime;
	float  setTime;
	bool  start;

	Temporizer():setTime(1000) {}
	Temporizer(float setTime) :setTime(setTime*1000) {
		
	}
	
	void Reset() {
		startTime = SDL_GetTicks();
	}

	void Start() 
	{
		start = true;
		startTime = SDL_GetTicks();
	}

	void Stop()
	{
		start = false;
	}

	bool Update() {
		if (start == true) 
		{
			if (SDL_GetTicks() - startTime >= setTime) 
			{
				startTime = SDL_GetTicks();
				return true;
			}
		}
		return false;
	}

};
