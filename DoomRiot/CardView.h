#pragma once
#include <SDL.h>

class CardView
{
	void SetMinionToRender(class Minion);
	void Render(SDL_Surface* surface);
};

