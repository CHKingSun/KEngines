#include "ShadowMap.h"
#include "../render/Shader.h"
#include "../object/Object3D.h"
#include "../math/vector/vec_default.h"
#include "../math/transform.h"

namespace KEngines { namespace KMaterial {
	const std::string ShadowMap::U_SHADOW_MAP("u_shadow_map");
	const std::string ShadowMap::U_LIGHT_MATRIX("u_light_matrix");

	KRenderer::Shader* ShadowMap::shadow_shader = nullptr;
	Kuint ShadowMap::shadow_count = 0;

	ShadowMap::ShadowMap(Kuint width, Kuint height) : t_size(width, height) {
		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			Log::error("Frame buffer is not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind to default frame buffer.

		if (shadow_count == 0) {
			assert(shadow_shader == nullptr);
			shadow_shader = new KRenderer::Shader(SHADER_PATH + "shadow_depth.vert", SHADER_PATH + "shadow_depth.frag");
		}
		++shadow_count;
	}

	ShadowMap::~ShadowMap() {
		if (glIsFramebuffer(frame_buffer)) glDeleteFramebuffers(1, &frame_buffer);
		if (glIsTexture(tex_id)) glDeleteTextures(1, &tex_id);

		--shadow_count;
		if (shadow_count == 0) {
			delete shadow_shader;
			shadow_shader = nullptr;
		}
	}

	void ShadowMap::bindDirectionLight(const std::vector<KVector::vec3>& bounds,
		const KVector::vec3& direction, const KVector::vec3& pos) {
		using namespace KVector;
		using namespace KMatrix;

		mat3 rot(vec3(1.f, 1.f, -1.f));
		if (direction == vec3(0.f)) {
			Log::debug("Direction is zero, use BackVector instead.");
		}
		else if (direction == ForwardVector) {
			rot[2][2] = 1.f;
		}

		rot = KFunction::rotate(BackVector.getAngle(direction),
			vec3::cross(BackVector, direction)).toMat3().transpose();

		proj = mat4(rot, rot * -pos);
		Kfloat x_min = FLT_MAX, y_min = FLT_MAX, z_min = FLT_MAX;
		Kfloat x_max = FLT_MIN, y_max = FLT_MIN, z_max = FLT_MIN;

		for (const auto& point : bounds) {
			vec4 p = proj * vec4(point, 1.f);

			x_min = min(x_min, p.x);
			y_min = min(y_min, p.y);
			z_min = min(z_min, p.z);
			x_max = max(x_max, p.x);
			y_max = max(y_max, p.y);
			z_max = max(z_max, p.z);
		}

		proj = KFunction::ortho(x_min, x_max, y_min, y_max, z_min, z_max) * proj;

		shadow_shader->bind();
		shadow_shader->bindUniformMat4f(U_LIGHT_MATRIX.c_str(), proj);
	}

	void ShadowMap::bindSpotLight(const KVector::vec3& direction,
		const KVector::vec3& pos, Kfloat z_near /* = 1.f */, Kfloat z_far /* = 100.f */) {
		using namespace KVector;
		using namespace KMatrix;

		mat3 rot(vec3(1.f, 1.f, -1.f));
		if (direction == vec3(0.f)) {
			Log::debug("Direction is zero, use BackVector instead.");
		}
		else if (direction == ForwardVector) {
			rot[2][2] = 1.f;
		}

		rot = KFunction::rotate(BackVector.getAngle(direction),
			vec3::cross(BackVector, direction)).toMat3().transpose();

		proj = KFunction::perspective(90.f, (Kfloat)t_size.x / (Kfloat)t_size.y, z_near, z_far);
		proj *= mat4(rot, rot * -pos);

		shadow_shader->bind();
		shadow_shader->bindUniformMat4f(U_LIGHT_MATRIX.c_str(), proj);
	}

	void ShadowMap::renderShadow(const KObject::Object3D* object,
		const KVector::ivec2& viewport)const {
		if (object == nullptr) {
			Log::debug("Object to get shadow is nullptr.");
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer); //Bind frame buffer
		glViewport(0, 0, t_size.x, t_size.y);
		glClear(GL_DEPTH_BUFFER_BIT);

		object->render(shadow_shader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind frame buffer
		glViewport(0, 0, viewport.x, viewport.y);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void ShadowMap::bindShadowTexture(const KRenderer::Shader* shader)const {
		if (shader == nullptr) {
			Log::error("The shader to bind unifrom is null!");
			return;
		}

		shader->bind();
		shader->bindUniformMat4f(U_LIGHT_MATRIX.c_str(), proj);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		shader->bindUniform1i(U_SHADOW_MAP.c_str(), 1);
	}
} }