#include "Font.h"
#include "../util/StringUtil.h"

namespace KEngines { namespace KObject {

	const std::string Font::default_file_name("C:/Windows/Fonts/msyh.ttc");
	const std::string Font::default_font_name("msyh_default");

	Font::Font(const std::string& font_name /* = default_font_name */,
		const std::string& file_name /* = default_file_name */,
		Kfloat font_scale /* = 30.f */) : file_name(file_name),
		font_name(font_name), font_scale(font_scale ), characters() {
		font_file = std::fopen(file_name.c_str(), "rb");
		if (!font_file) {
			Log::error("Load font file: ", file_name,
				" failed! Change it to default font");
			font_file = std::fopen(default_file_name.c_str(), "rb");
			if (!font_file) {
				Log::error("Load default font failed!");
				return;
			}
		}
		std::fseek(font_file, 0, SEEK_END);
		file_size = std::ftell(font_file);

		//loadBasicCharacters();
	}

	Font::Font(const std::string& file_name, Kfloat font_scale /* = 30.f */) :
		Font(KUtil::getFileName(file_name), file_name, font_scale) {}

	Font::~Font() {
		for (const auto& it : characters) {
			glDeleteTextures(1, &(it.second.tex_id));
		}
		std::fclose(font_file);
	}

	const Character& Font::operator[](Kint c) {
		if (characters.find(c) == characters.end()) loadCharacter(c);
		return characters[c];
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
} }