//
// Created by KingSun on 2018/07/14
//

#ifndef KENGINES_FONT_H
#define KENGINES_FONT_H

#include <GL/glew.h>
#include <unordered_map>
#include <memory>
#include <cstdio>

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <stb_truetype.h>
#endif // !STB_TRUETYPE_IMPLEMENTATION

#include "../KHeader.h"
#include "../Log.h"
#include "../render/Shader.h"

namespace KEngines {
	namespace KBuffer {
		class VertexArray;
		class VertexBuffer;
	}
}

namespace KEngines { namespace KObject {
	using namespace KEngines::KVector;

	struct Character {
		Character(): tex_id(0), x_advance(0) {};
		Character(Kuint tex_id, Kint x_advance,
			const ivec2& size, const ivec2& offset):
			tex_id(tex_id), x_advance(x_advance),
			size(size), offset(offset) {}

		Kuint tex_id;
		Kint x_advance;
		ivec2 size;
		ivec2 offset;
	};

	struct RenderText {
		std::wstring text;
		vec3 color;
		vec2 position;
		Kfloat scale;

		RenderText(const std::wstring& text, const vec3& color,
			const vec2& pos, Kfloat scale = 1.f) :
			text(text), color(color), position(pos), scale(scale) {}
	};

	//Maybe later it will be inherited form Object3D
	//You should set the right font(which has the charater)
	//if you want to display the right charater
	class Font {
	private:
		static const std::string default_font_name;
		static KRenderer::Shader* shader;
		static Kuint font_count;

		std::string font_name;
		FILE* font_file;
		Ksize file_size;
		Kfloat font_scale;

		std::unordered_map<Kint, Character> characters;
		std::vector<RenderText> render_texts;

		KBuffer::VertexArray* vao;
		KBuffer::VertexBuffer* vbo;

		void addCharacter(const stbtt_fontinfo& font_info, Kint c);
		void loadBasicCharacters();

	public:
		Font(const std::string& font_name = default_font_name, Kfloat font_scale = 32.f);
		~Font();

		void loadCharacter(Kint c);
		void loadText(const std::wstring& text);

		//This function will check whether the character is in characters,
		//Remember to enable blend before render.
		void addRenderText(const std::wstring& text, const vec3& color,
			const vec2& pos, Kfloat scale = 1.f);

		bool removeRenderText(const std::wstring& text);

		bool removeRenderText(Kuint index);

		bool changeRenderText(const std::wstring& old_text, const std::wstring& new_text);

		bool changeRenderText(Kuint old_index, const std::wstring& new_text);

		void render()const;

		inline static void setViewport(const ivec2& v) {
			if (font_count != 0) {
				shader->bind();
				shader->bindUniform2i("s_size", v);
			}
		}
	};
} }

#endif // !KENGINES_FONT_H

