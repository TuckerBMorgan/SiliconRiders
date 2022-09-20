#pragma once
#include <SDL.h>
#include <vector>
#include <SDL_ttf.h>
#include <unordered_map>

struct NumericalGlyph {
	SDL_Texture* texture;
	int width;
	int height;
};

struct RenderedText {
	SDL_Texture* texture;
	int width;
	int height;
};

class RenderCarePackage
{
private:
	SDL_Renderer* renderer;
	SDL_Texture* cardbackground;
	SDL_Texture* cardback;
	std::vector<NumericalGlyph> digits;
	TTF_Font* font;
	std::unordered_map<std::string, RenderedText> rendered_text;

public:

	RenderCarePackage(SDL_Renderer* renderer, SDL_Texture* cardbackground, SDL_Texture* cardback, std::vector<NumericalGlyph> digits, TTF_Font* font) : renderer(renderer), cardbackground(cardbackground), digits(digits), font(font), cardback(cardback) {

	}
	SDL_Renderer* GetRenderer() {
		return this->renderer;
	}

	SDL_Texture* GetCardbackgroundTexture() {
		return this->cardbackground;
	}

	SDL_Texture* GetCardBackTexture() {
		return this->cardback;
	}
	
	TTF_Font* GetFont() {
		return this->font;
	}

	std::vector<NumericalGlyph>& GetDigitTextures() {
		return this->digits;
	}

	RenderedText* GetFontTexture(std::string text) {
		if (this->rendered_text.count(text) == 0 ) {
			SDL_Color textColor;
			textColor.r = 0;
			textColor.g = 255;
			textColor.b = 0;
			textColor.a = 255;
			SDL_Surface* textSurface = TTF_RenderText_Solid(this->font, text.c_str(), textColor);
			auto mTexture = SDL_CreateTextureFromSurface(this->renderer, textSurface);
			RenderedText newText;
			newText.texture = mTexture;
			newText.height = textSurface->h;
			newText.width = textSurface->w;
			this->rendered_text[text] = newText;
		}
		return &this->rendered_text[text];
	}

};