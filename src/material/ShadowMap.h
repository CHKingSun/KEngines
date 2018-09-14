//
// Created by KingSun on 2018/09/07
//

#ifndef KENGINES_SHADOW_MAP_H
#define KENGINES_SHADOW_MAP_H

#include <GL/glew.h>
#include <vector>

#include "../KHeader.h"
#include "../math/vector/vec3.h"
#include "../math/matrix/mat4.h"

namespace KEngines { 
	namespace KRenderer { class Shader; }
	namespace KObject { class Object3D; }
}

namespace KEngines { namespace KMaterial {
	class ShadowMap {
	private:
		KVector::ivec2 t_size;

		Kuint frame_buffer;
		Kuint tex_id;

		KMatrix::mat4 proj;

		static const std::string U_SHADOW_MAP;
		static const std::string U_LIGHT_MATRIX;

		static KRenderer::Shader* shadow_shader;
		static Kuint shadow_count;

	public:
		ShadowMap(Kuint width, Kuint height);
		~ShadowMap();

		inline Kboolean isValid()const {
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
			Kboolean ret = (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER));
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return ret;
		}

		void bindLight(const std::vector<KVector::vec3>& bounds,
			const KVector::vec3& pos, const KVector::vec3& direction);

		void renderShadow(const KObject::Object3D* object, const KVector::ivec2& viewport)const;

		void bindShadowTexture(const KRenderer::Shader* shader)const;
	};
} }

#endif // !KENGINES_SHADOW_MAP_H
