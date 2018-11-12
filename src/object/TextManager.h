//
// Created by KingSun on 2018/11/10
//

#ifndef KENGINES_TEXT_MANAGER_H
#define KENGINES_TEXT_MANAGER_H

#include "../KHeader.h"
#include "../math/vector/vec_default.h"
#include "Font.h"

#include <unordered_map>
#include <functional>

namespace KEngines {
	namespace KBuffer {
		class VertexArray;
		class VertexBuffer;
	}
	namespace KRenderer { class Shader; }
	namespace KObject { class Text; }
}

namespace KEngines { namespace KObject {
	using namespace KVector;

	struct FontCount {
		KObject::Font* const font;
		Kuint use_count;

		FontCount(KObject::Font* font, Kuint use_count = 0) : font(font), use_count(use_count) {}
		~FontCount() { delete font; use_count = 0; }

		FontCount& operator++() { ++use_count; return *this; }
		FontCount& operator--() { --use_count; return *this; }
	};

	class TextManager {
		using ClickFunction = std::function<void(Text&)>;

	private:
		static std::unordered_map<std::string, FontCount> fonts;
		static std::vector<KObject::Text*> texts;

		static KRenderer::Shader* shader;
		static KBuffer::VertexArray* vao;
		static KBuffer::VertexBuffer* vbo;

		TextManager() = default;

	public:
		static void initial();

		static void render();

		static void destroy();

		static void setViewport(const ivec2& v);

		static Font* loadFont(const std::string& font_name, const std::string& file_name, Kfloat font_scale = 30.f);
		static Font* loadFont(const std::string& file_name, Kfloat font_scale = 30.f);

		static Font* getFont(const std::string& font_name);

		static Text* loadText(const std::wstring& text, const vec2& pos = vec2(0.f),
			const Color& color = WHITE, const Kfloat& scale = 1.f,
			const ClickFunction& listener = nullptr);

		static void attachFontToText(KObject::Font* font, KObject::Text* text);
		static void attachFontToText(const std::string& font_name, KObject::Text* text);

		static void keyEvent(Kint key, Kint action);
		static void mouseEvent(Kint button, Kint action);
		static void mouseWheelEvent(Kdouble yoffset);
		static void cursorEvent(Kdouble xpos, Kdouble ypos);
	};
} }

#endif // !KENGINES_TEXT_MANAGER_H

