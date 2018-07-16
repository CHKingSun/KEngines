#include "Font.h"

namespace KEngines { namespace KObject {

	const std::string Font::default_font_name("C:/Windows/Fonts/msyh.ttc");
	Kboolean Font::is_font_use = false;

	Font::Font(const std::string& font_name /* = default_font_name */, Kfloat scale /* = 32.f */) :
		font_name(font_name), scale(scale),
		vao(new KBuffer::VertexArray()), vbo(new KBuffer::VertexBuffer(
			KBuffer::VERTEX, sizeof(Kfloat) * 24, nullptr, GL_DYNAMIC_DRAW)),
		characters(new std::unordered_map<Kint, Character>()) {
		font_file = std::fopen(font_name.c_str(), "rb");
		if (!font_file) {
			Log::error("Load font file: ", font_name,
				" failed! Change it to default font");
			font_file = std::fopen(default_font_name.c_str(), "rb");
			if (!font_file) {
				Log::error("Load default font failed!");
				return;
			}
		}
		std::fseek(font_file, 0, SEEK_END);
		file_size = std::ftell(font_file);

		is_font_use = true;

		vao->allocate(vbo, 0, 4);

		loadBasicCharacters();
	}

	Font::~Font() {
		for (const auto& it : *characters) {
			glDeleteTextures(1, &(it.second.tex_id));
		}
		delete characters;
		std::fclose(font_file);
	}

	void Font::addCharacter(const stbtt_fontinfo& font_info, Kint c)const {
		Character ch;
		Kfloat y_scale = stbtt_ScaleForPixelHeight(&font_info, scale);
		Kuchar* bitmap = stbtt_GetCodepointBitmap(&font_info, 0.f, y_scale,
			c, &ch.size.x, &ch.size.y, &ch.offset.x, &ch.offset.y);
		stbtt_GetCodepointHMetrics(&font_info, c, &ch.x_advance, nullptr);
		ch.x_advance *= y_scale;

		glGenTextures(1, &ch.tex_id);
		glBindTexture(GL_TEXTURE_2D, ch.tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ch.size.x, ch.size.y,
			0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		stbtt_FreeBitmap(bitmap, font_info.userdata);

		characters->emplace(c, ch);
	}

	void Font::loadBasicCharacters()const {
		std::fseek(font_file, 0, SEEK_SET);
		const auto font_buffer = new Kuchar[file_size + 1];
		std::fread(font_buffer, file_size, 1, font_file);

		stbtt_fontinfo font_info;
		stbtt_InitFont(&font_info, font_buffer, stbtt_GetFontOffsetForIndex(font_buffer, 0));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (int c = 32; c < 127; ++c) {
			addCharacter(font_info, c);
		}

		delete[] font_buffer;
	}

	void Font::loadCharacter(Kint c)const {
		if (characters->find(c) != characters->end()) return;

		std::fseek(font_file, 0, SEEK_SET);
		const auto font_buffer = new Kuchar[file_size + 1];
		std::fread(font_buffer, file_size, 1, font_file);

		stbtt_fontinfo font_info;
		stbtt_InitFont(&font_info, font_buffer, stbtt_GetFontOffsetForIndex(font_buffer, 0));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		addCharacter(font_info, c);

		delete[] font_buffer;
	}

	void Font::loadText(const std::wstring& text)const {
		std::wstring tmp;
		tmp.reserve(text.size());
		for (const auto& c : text) {
			if (characters->find(c) == characters->end())
				tmp.push_back(c);
		}
		if (tmp.empty()) return;

		std::fseek(font_file, 0, SEEK_SET);
		const auto font_buffer = new Kuchar[file_size + 1];
		std::fread(font_buffer, file_size, 1, font_file);

		stbtt_fontinfo font_info;
		stbtt_InitFont(&font_info, font_buffer, stbtt_GetFontOffsetForIndex(font_buffer, 0));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (const auto& c : tmp) {
			addCharacter(font_info, c);
		}

		delete[] font_buffer;
	}

	void Font::renderText(const std::wstring& text, const vec3& color,
		Kfloat x, Kfloat y, Kfloat scale /* = 1.f */)const {
		glActiveTexture(GL_TEXTURE0); //default actived texture
		
		//Use reference so that the shder would not be destructed!
		const KRenderer::Shader& shader = getShader();
		//The function has bind shader.
		shader.bindUniform3f("text_color", color);
		vao->bind();
		vao->enableVertexArray();
		for (const auto& c : text) {
			Character ch = (*characters)[c];
			Kfloat xpos = x + ch.offset.x * scale;
			Kfloat ypos = y - (ch.size.y + ch.offset.y) * scale;

			Kfloat w = ch.size.x * scale;
			Kfloat h = ch.size.y * scale;

			const Kfloat vertices[] = {
				xpos    , ypos + h, 0.f, 0.f,
				xpos    , ypos    , 0.f, 1.f,
				xpos + w, ypos + h, 1.f, 0.f,
				xpos + w, ypos + h, 1.f, 0.f,
				xpos    , ypos    , 0.f, 1.f,
				xpos + w, ypos    , 1.f, 1.f
			};
			vbo->allocate(0, sizeof(vertices), vertices);
			glBindTexture(GL_TEXTURE_2D, ch.tex_id);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += ch.x_advance * scale;
		}
		vao->unBind();
		vao->disableVertexArray();
	}

	void Font::renderTextWithCheck(const std::wstring& text, const vec3& color,
		Kfloat x, Kfloat y, Kfloat scale /* = 1.f */)const {
		loadText(text);
		renderText(text, color, x, y, scale);
	}
} }