#pragma once

#include <SDL.h>


float DistanceBetweenPoints(SDL_FPoint a, SDL_FPoint b) {
	auto x = powf(a.x - b.x, 2);
	auto y = powf(a.y - b.y, 2);

	return sqrtf(x + y);
}

SDL_FPoint NormalizedVector(SDL_FPoint vector) {
	auto length = sqrtf(vector.x * vector.x + vector.y * vector.y);

	SDL_FPoint return_value;
	return_value.x = vector.x / length;
	return_value.y = vector.y / length;

	return return_value;
}

SDL_FPoint FPointSubtraction(SDL_FPoint a, SDL_FPoint b) {
	SDL_FPoint return_value;
	return_value.x = a.x - b.x;
	return_value.y = a.y - b.y;
	return return_value;
}