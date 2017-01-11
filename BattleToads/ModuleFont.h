#ifndef __MODULE_FONT_H__
#define __MODULE_FONT_H__
#include "Globals.h"
#include "Module.h"
#include "Point.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

class Text {
public:
	iPoint position;
	Text();
	~Text();

};

class ModuleFont 
{
public:
	SDL_Texture* Font;
	ModuleFont();
	~ModuleFont();
	void WriteText(int x, int y, std::string Text);
	std::string Alphabet = " ";

};

#endif