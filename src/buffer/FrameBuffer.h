//
// Created by KingSun on 2018/07/20
//

#ifndef KENGINES_FRAME_BUFFER_H
#define KENGINES_FRAME_BUFFER_H

#include <GL/glew.h>

#include "../KHeader.h"
#include "../render/Shader.h"

namespace KEngines { namespace KBuffer {
	class FrameBuffer {
	private:
		static KRenderer::Shader* shader;
		static Kuint frame_count;

		class VertexArray* vao;
		class VertexBuffer* vbo;

		Kuint frame_buffer;
		Kuint tex_id;

	public:
		FrameBuffer(Kuint width, Kuint height);
		~FrameBuffer();

		inline Kboolean isValid()const {
			glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
			Kboolean ret = (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER));
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return ret;
		}

		void begin()const;
		void end()const;

		void setOpacity(Kfloat opacity = 1.f)const;
		void render()const;

		inline static void setViewport(const KVector::ivec2& v) {
			if (frame_count != 0) {
				shader->bind();
				shader->bindUniform2i("s_size", v);
			}
		}
	};
} }

#endif // !KENGINES_FRAME_BUFFER_H
