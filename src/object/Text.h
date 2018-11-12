//
// Created by KingSun on 2018/11//06
// 

#ifndef KENGINES_TEXT_H
#define KENGINES_TEXT_H

#include "../KHeader.h"
#include "EventCallback.h"
#include "../math/vector/vec_default.h"

#include <functional>

namespace KEngines {
	namespace KBuffer {
		class VertexArray;
		class VertexBuffer;
	}
	namespace KRenderer { class Shader; }
	namespace KObject { class Font; }
}

namespace KEngines { namespace KObject {
	using namespace KVector;

	class Text : public EventCallback {
	public:
		using ClickFunction = std::function<void(Text&)>;

	private:
		std::wstring text;
		Color color;
		vec2 position;
		Kfloat scale;

		vec2 bound;
		Font* font; // Font will be deleted by other class.
		Kboolean is_focus;

		ClickFunction on_click;

		void updateBound();

	public:
		Text(const std::wstring& text, const vec2& pos = vec2(0.f),
			const Color& color = WHITE, const Kfloat& scale = 1.f,
			const ClickFunction& listener = nullptr) :
			text(text), position(pos), color(color),
			scale(scale), on_click(listener), is_focus(false),
			bound(0.f), font(nullptr) {}
		virtual ~Text() = default;

		void setClickListener(const ClickFunction& listener) { on_click = listener; }
		void setColor(const Color& new_color) { color = new_color; }
		void setPosition(const vec2& pos) { position = pos; }
		void setSacle(const Kfloat& scale) { this->scale = scale; }

		void setRenderText(const std::wstring& new_text);
		void setFont(KObject::Font* new_font);

		const std::wstring& getText()const { return text; }
		Font* const& getFont()const { return font; }
		const Color& getColor()const { return color; }
		const vec2& getPosition()const { return position; }
		const Kfloat& getScale()const { return scale; }

		virtual void keyEvent(Kint key, Kint action) override {};
		virtual void mouseEvent(Kint button, Kint action) override;
		virtual void mouseWheelEvent(Kdouble yoffset) override {};
		virtual void cursorEvent(Kdouble xpos, Kdouble ypos) override;
	};
} }

#endif // !KENGINES_TEXT_H
