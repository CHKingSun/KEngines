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
#include "../math/vector/vec4.h"
#include "../buffer/VertexArray.h"
#include "../render/Shader.h"

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
		static const std::string default_font_name;
		static Kboolean is_font_use;

		std::string font_name;
		FILE* font_file;
		Ksize file_size;
		Kfloat scale;
		std::unordered_map<Kint, Character>* characters;

		KBuffer::VertexArray* vao;
		KBuffer::VertexBuffer* vbo;

		static const KRenderer::Shader& getShader() {
			static const KRenderer::Shader shader(RES_PATH + "shaders/font.vert", RES_PATH + "shaders/font.frag");
			//How to delete once it constructed?
			//Count mode?
			shader.bind();
			return shader;
		}

		void addCharacter(const stbtt_fontinfo& font_info, Kint c)const;
		void loadBasicCharacters()const;

	public:
		Font(const std::string& font_name = default_font_name, Kfloat scale = 32.f);
		~Font();

		void loadCharacter(Kint c)const;
		void loadText(const std::wstring& text)const;

		//This function will not check whether the character is in characters,
		//Remember to enable blend before render.
		void renderText(const std::wstring& text, const vec3& color,
			Kfloat x, Kfloat y, Kfloat scale = 1.f)const;

		//This function will load the character if the it is not in characters,
		//Remember to enable blend before render.
		void renderTextWithCheck(const std::wstring& text, const vec3& color,
			Kfloat x, Kfloat y, Kfloat scale = 1.f)const;

		inline static void setViewport(const ivec2& v) {
			if(is_font_use) getShader().bindUniform2i("s_size", v);
		}
	};
} }

#endif // !KENGINES_FONT_H

