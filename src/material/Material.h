//
// Created by KingSun on 2018/08/02
//


#ifndef KENGINES_MATERIAL_H
#define KENGINES_MATERIAL_H

#include <vector>

#include "../KHeader.h"
#include "../math/vector/vec3.h"
#include "Texture.h"

namespace KEngines { namespace KRenderer { class Shader; } }

namespace KEngines { namespace KMaterial {
	using namespace KVector;

	class Material {
	private:
		static const std::string U_AMBIENT;
		static const std::string U_DIFFUSE;
		static const std::string U_SPECULAR;
		static const std::string U_SHININESS;
		static const std::string U_OPACITY;

	public:
		static const vec3 GREY;
		static const vec3 WHITE;
		static const vec3 BLACK;
	
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		Kfloat shininess;
		Kfloat opacity;

		std::vector<Texture*> textures;

	public:
		Material(const vec3& ma = GREY, const vec3& md = GREY,
				 const vec3& ms = GREY, Kfloat msh = 1.f, Kfloat mo = 1.f);
		Material(const std::vector<Texture*>& tex, const vec3& ma = GREY, const vec3& md = GREY,
			const vec3& ms = GREY, Kfloat msh = 1.f, Kfloat mo = 1.f);
		~Material();

		void bindUniform(const KRenderer::Shader* shader)const;

		void activeTextures(Kboolean enable = true)const;

		Kboolean addTexture(Texture* tex);

		Kboolean deleteTexture(Kuint index);
	};
} }

#endif // !KENGINES_MATERIAL_H
