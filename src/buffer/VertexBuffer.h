//
// Created by KingSun on 2018/07/13
//

#ifndef KENGINES_VERTEX_BUFFER_H
#define KENGINES_VERTEX_BUFFER_H

#include <GL/glew.h>
#include "../KHeader.h"

namespace KEngines { namespace KBuffer {
	enum BufferType {
		VERTEX = GL_ARRAY_BUFFER,
		INDEX  = GL_ELEMENT_ARRAY_BUFFER
	};

	class VertexBuffer {
	private:
		Kuint buffer_id;
		GLenum buffer_type;

	public:
		VertexBuffer(BufferType type, Ksize size, const void* data = nullptr,
					GLenum usage = GL_STATIC_DRAW): buffer_type(type) {
			glGenBuffers(1, &buffer_id);
			glBindBuffer(buffer_type, buffer_id);
			glBufferData(buffer_type, size, data, usage);
		}
		~VertexBuffer() {
			if (glIsBuffer(buffer_id)) glDeleteBuffers(1, &buffer_id);
		}

		void allocate(Kuint offset, Kuint size, const void* data)const {
			glBindBuffer(buffer_type, buffer_id);
			glBufferSubData(buffer_type, offset, size, data);
		}

		void bind()const {
			glBindBuffer(buffer_type, buffer_id);
		}

		void unBind()const {
			glBindBuffer(buffer_type, 0);
		}
	};
} }

#endif // !KENGINES_VERTEX_BUFFER_H

