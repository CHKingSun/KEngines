//
// Created by KingSun on 2018/08/02
//

#ifndef KENGINES_TEXTURE_H
#define KENGINES_TEXTURE_H

#include "../KHeader.h"
#include <unordered_map>

namespace KEngines { namespace KRenderer { class Shader; } }

namespace KEngines { namespace KMaterial {
	enum TextureType {
		NONE = 0, AMBIENT, DIFFUSE, SPECULAR
	};

	struct TextureCount {
		TextureCount(Kuint id) : tex_id(id), count(1) {}

		Kuint tex_id;
		Kuint count;
	};
	
	class Texture {
	private:
		Kuint tex_id;
		Kuint tex_unit;
		TextureType type;
		std::string path;

		std::string u_tex;
		std::string u_type;

		static std::unordered_map<std::string, TextureCount> tex_msg;

	public:
		//Note: tex_unit is the sampler unit(GL_TEXTUREi), usually it is 0.
		//If you want to multiply texture in one render, then you should set tex_unit in different value.
		Texture(const std::string& path, TextureType type = AMBIENT, Kuint tex_unit = 0);
		~Texture();

		static Kuint getTextureId(const std::string& path);

		//Maybe we should check whether the unit is valid.
		void setTextureUnit(Kuint unit);

		void bindUniform(const KRenderer::Shader* shader)const;

		void unActive(const KRenderer::Shader* shader)const;
	};
} }

#endif // !KENGINES_TEXTURE_H
