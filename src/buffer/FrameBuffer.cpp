#include "FrameBuffer.h"
#include "VertexArray.h"
#include "../render/Shader.h"

namespace KEngines { namespace KBuffer {

	KRenderer::Shader* FrameBuffer::shader = nullptr;
	Kuint FrameBuffer::frame_count = 0;

	FrameBuffer::FrameBuffer(Kuint width, Kuint height) : vao(nullptr), vbo(nullptr) {
		const Kfloat vertices[] = {
			-1.f, -1.f, 0.f, 0.f,
			 1.f, -1.f, 1.f, 0.f,
			-1.f,  1.f, 0.f, 1.f,
			-1.f,  1.f, 0.f, 1.f,
			 1.f, -1.f, 1.f, 0.f,
			 1.f,  1.f, 1.f, 1.f,

			 //0.f,  0.f,  0.f, 0.5f,
			 //1.f,  0.f, 0.5f, 0.5f,
			 //0.f, -1.f,  0.f,  0.f,
			 //0.f, -1.f,  0.f,  0.f,
			 //1.f,  0.f, 0.5f, 0.5f,
			 //1.f, -1.f, 0.5f,  0.f

			 0.f,  0.f, 0.f, 1.f,
			 1.f,  0.f, 1.f, 1.f,
			 0.f, -1.f, 0.f, 0.f,
			 0.f, -1.f, 0.f, 0.f,
			 1.f,  0.f, 1.f, 1.f,
			 1.f, -1.f, 1.f, 0.f
		};

		if (frame_count == 0) {
			assert(shader == nullptr);
			shader = new KRenderer::Shader(SHADER_PATH + "frame.vert", SHADER_PATH + "frame.frag");
		}
		++frame_count;

		shader->bind();
		vao = new VertexArray();
		vbo = new VertexBuffer(VERTEX, sizeof(vertices), vertices);
		vao->allocate(vbo, 0, 4);

		glGenFramebuffers(1, &frame_buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);

		//Kuint render_buffer;
		//glGenRenderbuffers(1, &render_buffer);
		//glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w_size.x, w_size.y);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
			Log::error("Frame buffer is not complete!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind to default frame buffer.
	}

	FrameBuffer::~FrameBuffer() {
		delete vbo;
		delete vao;

		if (glIsFramebuffer(frame_buffer)) glDeleteFramebuffers(1, &frame_buffer);
		if (glIsTexture(tex_id)) glDeleteTextures(1, &tex_id);

		--frame_count;
		if (frame_count == 0) {
			delete shader;
			shader = nullptr;
		}
	}

	void FrameBuffer::begin()const {
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer); //Bind frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::end()const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //Bind frame buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FrameBuffer::setOpacity(Kfloat opacity /* = 1.f */)const {
		shader->bind();
		shader->bindUniform1f("u_opacity", opacity);
	}

	void FrameBuffer::render()const {
		shader->bind();
		vao->bind();
		vao->enableVertexArray();
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}
} }