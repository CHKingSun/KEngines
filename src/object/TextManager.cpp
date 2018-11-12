#include "TextManager.h"
#include "../render/Shader.h"
#include "../buffer/VertexArray.h"
#include "../buffer/VertexBuffer.h"
#include "Font.h"
#include "Text.h"

namespace KEngines { namespace KObject {
	std::unordered_map<std::string, FontCount> TextManager::fonts;
	std::vector<KObject::Text*> TextManager::texts;

	KRenderer::Shader* TextManager::shader = nullptr;
	KBuffer::VertexArray* TextManager::vao = nullptr;
	KBuffer::VertexBuffer* TextManager::vbo = nullptr;

	void TextManager::initial() {
		if(shader == nullptr)
		shader = new KRenderer::Shader(SHADER_PATH + "font.vert", SHADER_PATH + "font.frag");

		if(vao == nullptr)
		vao = new KBuffer::VertexArray();

		if(vbo == nullptr)
		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(Kfloat) * 24, nullptr, GL_DYNAMIC_DRAW);
		vao->allocate(vbo, 0, 4);

		if(fonts.find(Font::default_font_name) == fonts.end())
		fonts.emplace(Font::default_font_name, new Font());
	}

	void TextManager::destroy() {
		for (auto& text : texts) {
			delete text; text = nullptr;
		}
		texts.clear();

		fonts.clear();

		delete vbo; vbo = nullptr;
		delete vao; vao = nullptr;
		delete shader; shader = nullptr;
	}

	void TextManager::render() {
		glActiveTexture(GL_TEXTURE0); //default actived texture

		shader->bind();
		vao->bind();
		//vao->enableVertexArray();
		for (const auto& text : texts) {
			auto font = text->getFont();
			if (font == nullptr) {
				auto& f_count = fonts.at(Font::default_font_name);
				++f_count;
				text->setFont(f_count.font);
			}
			else if (fonts.find(font->getFontName()) == fonts.end()) {
				fonts.emplace(font->getFontName(), FontCount(font, 1));
			}

			shader->bindUniform3f("text_color", text->getColor());
			Kfloat x = text->getPosition().x; // x will change.
			for (const auto& c : text->getText()) {
				const Character& ch = font->at(c);
				Kfloat xpos = x + ch.offset.x * text->getScale();
				Kfloat ypos = text->getPosition().y + font->getFontScale()
					+ ch.offset.y * text->getScale();

				Kfloat w = ch.size.x * text->getScale();
				Kfloat h = ch.size.y * text->getScale();

				const Kfloat vertices[] = {
					xpos    , ypos    , 0.f, 0.f,
					xpos    , ypos + h, 0.f, 1.f,
					xpos + w, ypos    , 1.f, 0.f,
					xpos + w, ypos    , 1.f, 0.f,
					xpos    , ypos + h, 0.f, 1.f,
					xpos + w, ypos + h, 1.f, 1.f
				};
				vbo->allocate(0, sizeof(vertices), vertices);
				glBindTexture(GL_TEXTURE_2D, ch.tex_id);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				x += ch.x_advance * text->getScale();
			}
		}
		vao->unBind();
		//vao->disableVertexArray();
	}

	void TextManager::setViewport(const ivec2& v) {
		if (shader == nullptr) return;
		shader->bind();
		shader->bindUniform2i("s_size", v);
	}

	Font* TextManager::getFont(const std::string& font_name) {
		if (fonts.find(font_name) == fonts.end()) return nullptr;
		return fonts.at(font_name).font;
	}

	Font* TextManager::loadFont(const std::string& font_name,
		const std::string& file_name, Kfloat font_scale /* = 1.f */) {
		auto font = new Font(font_name, file_name, font_scale);
		fonts.emplace(font_name, font);
		return font;
	}

	Font* TextManager::loadFont(const std::string& file_name, Kfloat font_scale /* = 1.f */) {
		auto font = new Font(file_name, font_scale);
		fonts.emplace(font->getFontName(), font);
		return font;
	}

	Text* TextManager::loadText(const std::wstring& text, const vec2& pos /* = vec2(0.f) */,
		const Color& color /* = WHITE */, const Kfloat& scale /* = 1.f */,
		const ClickFunction& listener /* = nullptr */) {
		auto t = new Text(text, pos, color, scale, listener);
		texts.emplace_back(t);
		return t;
	}

	void TextManager::attachFontToText(KObject::Font* font, KObject::Text* text) {
		if (text == nullptr) return;
		if (font != nullptr) {
			if (fonts.find(font->getFontName()) != fonts.end()) {
				++fonts.at(font->getFontName());
			}
			else {
				fonts.emplace(font->getFontName(), font);
			}
		}
		auto t_font = text->getFont();
		if (t_font != nullptr && fonts.find(t_font->getFontName()) != fonts.end()) {
			--fonts.at(t_font->getFontName());
		}

		text->setFont(font);
	}

	void TextManager::attachFontToText(const std::string& font_name, KObject::Text* text) {
		if (text == nullptr || fonts.find(font_name) == fonts.end()) return;
		auto t_font = text->getFont();
		if (t_font != nullptr && fonts.find(t_font->getFontName()) != fonts.end()) {
			--fonts.at(t_font->getFontName());
		}

		text->setFont((++fonts.at(font_name)).font);
	}

	void TextManager::keyEvent(Kint key, Kint action) {
		for (auto& text : texts) text->keyEvent(key, action);
	}

	void TextManager::mouseEvent(Kint button, Kint action) {
		for (auto& text : texts) text->mouseEvent(button, action);
	}

	void TextManager::mouseWheelEvent(Kdouble yoffset) {
		for (auto& text : texts) text->mouseWheelEvent(yoffset);
	}

	void TextManager::cursorEvent(Kdouble xpos, Kdouble ypos) {
		for (auto& text : texts) text->cursorEvent(xpos, ypos);
	}
} }