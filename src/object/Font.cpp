#include "Font.h"
#include "../buffer/VertexArray.h"

namespace KEngines { namespace KObject {

	const std::string Font::default_font_name("C:/Windows/Fonts/msyh.ttc");
	KRenderer::Shader* Font::shader = nullptr;
	Kuint Font::font_count = 0;

	Font::Font(const std::string& font_name /* = default_font_name */, Kfloat font_scale /* = 32.f */) :
		font_name(font_name), font_scale(font_scale),
		vao(new KBuffer::VertexArray()), vbo(new KBuffer::VertexBuffer(
			KBuffer::VERTEX, sizeof(Kfloat) * 24, nullptr, GL_DYNAMIC_DRAW)),
		characters(), render_texts() {
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

		vao->allocate(vbo, 0, 4);

		loadBasicCharacters();
		
		if (font_count == 0) {
			assert(shader == nullptr);
			shader = new KRenderer::Shader(SHADER_PATH + "font.vert", SHADER_PATH + "font.frag");
		}
		++font_count;
	}

	Font::~Font() {
		for (const auto& it : characters) {
			glDeleteTextures(1, &(it.second.tex_id));
		}
		std::fclose(font_file);

		delete vbo;
		delete vao;

		--font_count;
		if (font_count == 0) {
			delete shader;
			shader = nullptr;
		}
	}

	void Font::addCharacter(const stbtt_fontinfo& font_info, Kint c) {
		Character ch;
		Kfloat y_scale = stbtt_ScaleForPixelHeight(&font_info, font_scale);
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

		characters.emplace(c, ch);
	}

	void Font::loadBasicCharacters() {
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

	void Font::loadCharacter(Kint c) {
		if (characters.find(c) != characters.end()) return;

		std::fseek(font_file, 0, SEEK_SET);
		const auto font_buffer = new Kuchar[file_size + 1];
		std::fread(font_buffer, file_size, 1, font_file);

		stbtt_fontinfo font_info;
		stbtt_InitFont(&font_info, font_buffer, stbtt_GetFontOffsetForIndex(font_buffer, 0));
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		addCharacter(font_info, c);

		delete[] font_buffer;
	}

	void Font::loadText(const std::wstring& text) {
		std::wstring tmp;
		tmp.reserve(text.size());
		for (const auto& c : text) {
			if (characters.find(c) == characters.end())
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

	void Font::addRenderText(const std::wstring& text, const vec3& color,
		const vec2& pos, Kfloat scale /* = 1.f */) {
		loadText(text);
		render_texts.emplace_back(text, color, pos, scale);
	}

	bool Font::removeRenderText(const std::wstring& text) {
		for (auto it = render_texts.begin(); it != render_texts.end(); ++it) {
			if (it->text == text) {
				render_texts.erase(it);
				return true;
			}
		}

		return false;
	}

	bool Font::removeRenderText(Kuint index) {
		if (index < render_texts.size()) {
			render_texts.erase(render_texts.begin() + index);
			return true;
		}

		return false;
	}

	bool Font::changeRenderText(const std::wstring& old_text, const std::wstring& new_text) {
		for (auto& render_text : render_texts) {
			if (render_text.text == old_text) {
				render_text.text = new_text;
				return true;
			}
		}

		return false;
	}

	bool Font::changeRenderText(Kuint old_index, const std::wstring& new_text) {
		if (old_index < render_texts.size()) {
			render_texts[old_index].text = new_text;
			return true;
		}

		return false;
	}

	void Font::render()const {
		glActiveTexture(GL_TEXTURE0); //default actived texture

		shader->bind();
		vao->bind();
		//vao->enableVertexArray();
		for (const auto& text : render_texts) {
			shader->bindUniform3f("text_color", text.color);
			Kfloat x = text.position.x; // x will change.
			for (const auto& c : text.text) {
				const Character& ch = characters.at(c);
				Kfloat xpos = x + ch.offset.x * text.scale;
				Kfloat ypos = text.position.y - (ch.size.y + ch.offset.y) * text.scale;

				Kfloat w = ch.size.x * text.scale;
				Kfloat h = ch.size.y * text.scale;

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

				x += ch.x_advance * text.scale;
			}
		}
		vao->unBind();
		//vao->disableVertexArray();
	}
} }