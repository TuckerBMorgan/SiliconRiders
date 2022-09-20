#include "Log.h"
#include "RenderCarePackage.h"

void Log::Render(RenderCarePackage* render_care_package) {
	SDL_Rect text_render;
	text_render.x = 0;
	text_render.y = 0;

	for (auto& message : this->messages) {
		auto name_texture = render_care_package->GetFontTexture(message.message);
		text_render.w = name_texture->width;
		text_render.h = name_texture->height;
		SDL_RenderCopy(render_care_package->GetRenderer(), name_texture->texture, NULL, &text_render);
		text_render.y += text_render.h;
		message.duration -= 1;
	}
}