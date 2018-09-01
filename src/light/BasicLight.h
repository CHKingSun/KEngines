//
// Created by KingSun on 2018/08/11
//

#ifndef KENGINES_BASIC_LIGHT_H
#define KENGINES_BASIC_LIGHT_H

#include "../KHeader.h"
#include "../math/vector/vec_default.h"

namespace KEngines { namespace KRenderer { class Shader; } }

namespace KEngines { namespace KLight {
	enum LightType {
		BASIC = 1, DIRECTION, POINT, SPOT
	};

	using namespace KVector;

	//It seems like a structure but I define it to a class(for virtual).
	class BasicLight {
	private:
		const LightType type;

	protected:
		//Note: bind_id default is 0.
		//If you want to multiply lights in one render, then you should set bind_id in different value.
		Kuint bind_id;
		std::string u_enable;
		std::string u_intensity;
		std::string u_ambient;

	public:
		Kfloat intensity;
		vec3 ambient;

	protected:
		BasicLight(const vec3& ambient, Kfloat intensity, LightType type, Kuint bind_id,
			const char* u_enable , const char* u_intensity, const char* u_ambient) :
			ambient(ambient), intensity(intensity), type(type), bind_id(bind_id),
			u_enable(u_enable), u_ambient(u_ambient), u_intensity(u_intensity) {}

	public:
		BasicLight(const vec3& ambient = GREY, Kfloat intensity = 1.f, Kuint bind_id = 0) :
			BasicLight(ambient, intensity, BASIC, bind_id,
				"u_bLights[i].enable", "u_bLights[i].intensity", "u_bLights[i].ambient") {
			setBindId(bind_id);
		}
		virtual ~BasicLight() = default;

		const LightType& getLightType()const { return type; }

		void active(const KRenderer::Shader* shader, Kboolean enable = true)const;

		//Maybe we should check whether bind_id is valid.
		virtual void setBindId(Kuint bind_id);

		virtual void bindUniform(const KRenderer::Shader* shader)const;

		//Note: do nothing in BasicLight and DirectionLight class.
		virtual void bindPosition(const KRenderer::Shader* shader)const {};

		//Note: do nothing in BasicLight and PointLight class.
		virtual void bindDirection(const KRenderer::Shader* shader)const {};
	};
} }

#endif // !KENGINES_BASIC_LIGHT_H

