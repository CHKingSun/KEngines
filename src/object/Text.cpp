#include "Text.h"

#include "../render/Shader.h"
#include "Font.h"

#ifndef GLFW_RELEASE
#define GLFW_RELEASE 0
#endif // !GLFW_RELEASE

#ifndef GLFW_MOUSE_BUTTON_LEFT
#define GLFW_MOUSE_BUTTON_LEFT 0
#endif // !GLFW_MOUSE_BUTTON_LEFT

namespace KEngines { namespace KObject {
	void Text::setRenderText(const std::wstring& new_text) {
		if (new_text == text) return;
		text = new_text;
		if (font != nullptr) {
			font->loadText(text);
			updateBound();
		}
	}

	void Text::setFont(KObject::Font* new_font) {
		if (new_font == font) return;
		font = new_font;
		if (font != nullptr) {
			font->loadText(text);
			updateBound();
		}
	}

	// Note: font will not be nullptr when the function called.
	void Text::updateBound() {
		bound = vec2(0.f);
		for (const auto& c : text) {
			const auto& ch = font->at(c);
			bound.x += ch.x_advance * scale;
			bound.y = max(bound.y, ch.size.y* scale);
		}
	}

	void Text::mouseEvent(Kint button, Kint action) {
		if (is_focus && action == GLFW_RELEASE
			&& button == GLFW_MOUSE_BUTTON_LEFT) {
			if(on_click != nullptr) on_click(*this);
		}
	}

	void Text::cursorEvent(Kdouble xpos, Kdouble ypos) {
		is_focus = xpos >= position.x
				&& xpos <= position.x + bound.x
				&& ypos >= position.y
				&& ypos <= position.y + bound.y;
	}
} }