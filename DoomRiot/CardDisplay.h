#pragma once
#include <SDL.h>

class CardDisplay
{
private: 
	SDL_Rect screen_rectangle;
	SDL_FPoint offset;
	SDL_FPoint current_offset;

	bool should_be_offsetting;
	float scale;
	float current_scale;

public:
	CardDisplay(SDL_Rect screen_rectangle, float scale) : screen_rectangle(screen_rectangle), should_be_offsetting(false), scale(scale), current_scale(scale){
		this->offset.x = 0.0;
		this->offset.y = 0.0;
		this->current_offset.x = 0.0;
		this->current_offset.y = 0.0;
	};

	void SetShouldBeOffSetting(bool new_offsetting_value) {
		this->should_be_offsetting = new_offsetting_value;
		if (this->should_be_offsetting == false) {
			this->offset.x = 0;
			this->offset.y = 0;
		}
	}


	SDL_Rect CalculateCurrentScreenRectangle() {
		SDL_Rect rect;
		rect.x = this->screen_rectangle.x;
		rect.y = this->screen_rectangle.y;
		rect.x += this->current_offset.x;
		rect.y += this->current_offset.y;
		rect.w = this->screen_rectangle.w * this->current_scale;
		rect.h = this->screen_rectangle.h * this->current_scale;
		return rect;
	}


	void AddMoreToOffset(SDL_FPoint point) {
		this->offset.x += point.x;
		this->offset.y += point.y;
	}

	void SetScale(float new_scale) {
		this->scale = new_scale;
	}

	void Tick(float delta_time);
};

