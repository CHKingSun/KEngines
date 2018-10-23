//
// Created by KingSun on 2018/08/13
//

#ifndef KENGINES_DIRECTION_LIGHT_H
#define KENGINES_DIRECTION_LIGHT_H

#include <vector>

#include "BasicLight.h"
#include "../math/matrix/quaternion.h"
#include "../Log.h"

namespace KEngines { namespace KLight {
	class DirectionLight : public BasicLight {
	protected:
		std::string u_direction;
		std::string u_diffuse;
		std::string u_specular;

		vec3 direction;

	public:
		vec3 diffuse;
		vec3 specular;

	public:
		DirectionLight(const vec3& direction = vec3(0.f, 0.f, -1.f), const vec3& ambient = GREY,
			const vec3& diffuse = GREY, const vec3& specular = GREY,
			Kfloat intensity = 1.f, Kuint bind_id = 0) :
			BasicLight(ambient, intensity, DIRECTION, bind_id, "u_dLights[i].enable",
				"u_dLights[i].intensity", "u_dLights[i].ambient"),
			direction(direction), diffuse(diffuse), specular(specular),
			u_direction("u_dLights[i].direction"), u_diffuse("u_dLights[i].diffuse"),
			u_specular("u_dLights[i].specular") {
			this->direction.normalize();
			setBindId(bind_id);
		}
		~DirectionLight() = default;

		void setDirection(const vec3& direction) {
			if (direction == vec3(0.f)) {
				Log::error("Direction cannot be zero vector");
				return;
			}

			this->direction = direction;
			this->direction.normalize();
		}
		const vec3& getDirection()const { return direction; }

		void rotate(const KMatrix::quaternion& q) {
			direction = q * direction;
		}

		void setBindId(Kuint bind_id)override;

		void bindUniform(const KRenderer::Shader* shader)const override;

		void bindDirection(const KRenderer::Shader* shader)const override;
	};
} }

#endif // !KENGINES_DIRECTION_LIGHT_H

