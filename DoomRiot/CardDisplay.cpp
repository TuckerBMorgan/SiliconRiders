#include <math.h>
#include <iostream>

#include "CardDisplay.h"
#include "sdl_math.h"


float signum(float value) {
	if (value > 0.0) {
		return 1.0;
	}

	if (value == 0.0) {
		return 0.0;
	}

	return -1.0;
}

void CardDisplay::Tick(float delta) {
	
	float distance = fabsf(this->current_scale - this->scale);

	if (distance > SDL_FLT_EPSILON) {
		this->current_scale = this->current_scale + signum(this->scale - this->current_scale) * delta;		
	}
	
		
	distance = DistanceBetweenPoints(this->current_offset, this->offset);
	if (distance > SDL_FLT_EPSILON) {
		auto sub_point = FPointSubtraction(this->offset, this->current_offset);
		SDL_FPoint normal = NormalizedVector(sub_point);
		this->current_offset.x += normal.x * 0.1;// *delta;
		this->current_offset.y += normal.y * 0.1;// *delta;
	}
	

}