#include <iostream>
#include <string>
#include "ModuleFont.h"
#include "ModuleTextures.h"
#include "Application.h"
#include <iostream>
#include <string>

using namespace std;


ModuleFont::ModuleFont()
{
	Font = App->textures->Load("rtype/BattletoadSprites/level1.png");;
}


ModuleFont::~ModuleFont()
{
}

void ModuleFont::WriteText(int x, int y, string text) {
	
}