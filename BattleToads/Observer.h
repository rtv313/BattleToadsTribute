#pragma once

class Observer
{
public:
	virtual ~Observer() {};
	virtual void onNotify(GameEvent event) {};
	virtual void onNotify(GameEvent event, int downPosition) {};
};
