//
// Created by KingSun on 2018/07/14
//

#ifndef KENGINES_FONT_H
#define KENGINES_FONT_H

#include <GL/glew.h>
#include <unordered_map>

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <stb_truetype.h>
#endif // !STB_TRUETYPE_IMPLEMENTATION

#include "../KHeader.h"
#include "../Log.h"
#include "../math/vector/vec2.h"

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

	//Maybe later it will be inherited form Object3D
	//You should set the right font(which has the charater)
	//if you want to display the right charater
	class Font {
	private:
		std::string font_name;
		std::string file_name;
		FILE* font_file;
		Ksize file_size;
		Kfloat font_scale;

		std::unordered_map<Kint, Character> characters;

		void addCharacter(const stbtt_fontinfo& font_info, Kint c);
		void loadBasicCharacters();

	public:
		static const std::string default_font_name;
		static const std::string default_file_name;

	public:
		Font(const std::string& font_name = default_font_name,
			const std::string& file_name = default_file_name, Kfloat font_scale = 30.f);
		Font(const std::string& file_name, Kfloat font_scale = 30.f);
		~Font();

		Kboolean operator==(const Font& font)const { return this->font_name == font.font_name; }

		// If character is not exist, then the function will add it.
		// So the function is not const but return data is const.
		const Character& operator[](Kint c);
		const Character& at(Kint c) { return(*this)[c]; }

		const std::string& getFontName()const { return font_name; }
		const Kfloat& getFontScale()const { return font_scale; }

		void loadCharacter(Kint c);
		void loadText(const std::wstring& text);
	};
} }

#endif // !KENGINES_FONT_H

