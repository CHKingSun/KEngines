//
// Created by KingSun on 2018/08/13
//

#ifndef KENGINES_SPOT_LIGHT_H
#define KENGINES_SPOT_LIGHT_H

#include "BasicLight.h"
#include "../Log.h"
#include "../math/matrix/quaternion.h"

namespace KEngines {
	namespace KLight {
		class SpotLight : public BasicLight {
		protected:
			std::string u_position;
			std::string u_direction;
			std::string u_diffuse;
			std::string u_specular;
			std::string u_kq;
			std::string u_kl;
			std::string u_kc;
			std::string u_inner_cutoff;
			std::string u_outer_cutoff;

			vec3 position;
			vec3 direction;

		public:
			vec3 diffuse;
			vec3 specular;
			Kfloat kq, kl, kc;
			Kfloat inner_cutoff, outer_cutoff;

		public:
			SpotLight(const vec3& pos = vec3(), const vec3& direction = vec3(0.f, 0.f, -1.f),
				const vec3& ambient = GREY, const vec3& diffuse = GREY,
				const vec3& specular = GREY, Kfloat intensity = 1.f,
				Kfloat kq = 0.032f, Kfloat kl = 0.09f, Kfloat kc = 1.f,
				Kfloat inner_cutoff = 0.95f, Kfloat outer_cutoff = 0.87f, Kuint bind_id = 0) :
				BasicLight(ambient, intensity, SPOT, bind_id, "u_sLights[i].enable",
					"u_sLights[i].intensity", "u_sLights[i].ambient"),
				position(pos), direction(direction), diffuse(diffuse), specular(specular),
				kq(kq), kl(kl), kc(kc), inner_cutoff(inner_cutoff), outer_cutoff(outer_cutoff),
				u_position("u_sLights[i].position"), u_direction("u_sLights[i].direction"),
				u_diffuse("u_sLights[i].diffuse"), u_specular("u_sLights[i].specular"),
				u_kq("u_sLights[i].kq"), u_kl("u_sLights[i].kl"), u_kc("u_sLights[i].kc"),
				u_inner_cutoff("u_sLights[i].inner_cutoff"),
				u_outer_cutoff("u_sLights[i].outer_cutoff") {
				setBindId(bind_id);
			}
			~SpotLight() = default;

			void setPosition(const vec3& pos) { position = pos; }

			void translate(const vec3& v) { position += v; }

			void setDirection(const vec3& direction) {
				if (direction == vec3(0.f)) {
					Log::error("Direction cannot be zero vector");
					return;
				}

				this->direction = direction;
			}

			void rotate(const KMatrix::quaternion& q) { direction = q * direction; }

			void setBindId(Kuint bind_id)override;

			void bindUniform(const KRenderer::Shader* shader)const override;

			void bindPosition(const KRenderer::Shader* shader)const override;

			void bindDirection(const KRenderer::Shader* shader)const override;
		};
	}
}

#endif // !KENGINES_POINT_LIGHT_H
