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
		AMBIENT = 1, DIFFUSE, SPECULAR
	};

	struct TextureCount {
		TextureCount(Kuint id) : tex_id(id), count(1) {}

		Kuint tex_id;
		Kuint count;
	};
	
	class Texture {
	private:
		Kuint tex_id;
		mutable Kuint active_id;
		TextureType type;
		std::string path;

		static Kint max_tex_num;
		static std::unordered_map<std::string, TextureCount> tex_msg;

		const static std::string TEX_HEAD; // = "u_textures[";
		const static std::string TEX_TEXTURE; // = "].tex";
		const static std::string TEX_TYPE; // = "].type";
		//const static std::string TEX_ENABLE; // = "].enable";

	public:
		Texture(const std::string& path, TextureType type = AMBIENT);
		~Texture();

		static Kuint getTextureId(const std::string& path);

		void bindUniform(const KRenderer::Shader* shader, Kuint active_id)const;

		void active(Kboolean enable = true);
	};
} }

#endif // !KENGINES_TEXTURE_H
