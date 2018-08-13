//
// Created by KingSun on 2018/08/13
//

#ifndef KENGINES_POINT_LIGHT_H
#define KENGINES_POINT_LIGHT_H

#include "BasicLight.h"

namespace KEngines {
	namespace KLight {
		class PointLight : public BasicLight {
		protected:
			std::string u_position;
			std::string u_diffuse;
			std::string u_specular;
			std::string u_kq;
			std::string u_kl;
			std::string u_kc;

			vec3 position;

		public:
			vec3 diffuse;
			vec3 specular;
			Kfloat kq, kl, kc;

		public:
			PointLight(const vec3& pos = vec3(), const vec3& ambient = GREY,
				const vec3& diffuse = GREY, const vec3& specular = GREY,
				Kfloat intensity = 1.f, Kfloat kq = 0.032f, Kfloat kl = 0.09f,
				Kfloat kc = 1.f, Kuint bind_id = 0) :
				BasicLight(ambient, intensity, POINT, bind_id, "u_pLights[i].enable",
					"u_pLights[i].intensity", "u_pLights[i].ambient"),
				position(pos), diffuse(diffuse), specular(specular), kq(kq), kl(kl), kc(kc), 
				u_position("u_pLights[i].position"), u_diffuse("u_pLights[i].diffuse"),
				u_specular("u_pLights[i].specular"), u_kq("u_pLights[i].kq"),
				u_kl("u_pLights[i].kl"), u_kc("u_pLights[i].kc") {
				setBindId(bind_id);
			}
			~PointLight() = default;

			void setPosition(const vec3& pos) { position = pos; }

			void translate(const vec3& v) { position += v; }

			void setBindId(Kuint bind_id)override;

			void bindUniform(const KRenderer::Shader* shader)const override;

			void bindPosition(const KRenderer::Shader* shader)const override;
		};
	}
}

#endif // !KENGINES_POINT_LIGHT_H
