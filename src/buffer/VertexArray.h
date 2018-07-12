//
// Created by KingSun on 2018/07/13
//

#ifndef KENGINES_VERTEX_ARRAY_H
#define KENGINES_VERTEX_ARRAY_H

#include <unordered_set>
#include "../math/vector/vec3.h"
#include "VertexBuffer.h"

namespace KEngines { namespace KBuffer {
	class VertexArray {
	private:
		Kuint array_id;
		std::unordered_set<Kint>* locations;

	public:
		VertexArray() : locations(nullptr) {
			glGenVertexArrays(1, &array_id);
			glBindVertexArray(array_id);
		}
		~VertexArray() {
			if (glIsVertexArray(array_id)) glDeleteVertexArrays(1, &array_id);
			delete locations;
		}

		void allocate(const VertexBuffer* vbo, Kint location, Kuint size, GLenum type = GL_FLOAT,
			Kboolean normalized = false, Kuint stride = 0, Kuint offset = 0) {
			bind();
			vbo->bind();
			if (locations == nullptr) locations = new std::unordered_set<Kint>();
			locations->insert(location);
			glVertexAttribPointer(location, size, type, normalized, stride, (const void*)offset);
		}

		void setVertexAttrib3f(Kint location, const KVector::vec3& v)const {
			glVertexAttrib3fv(location, v.data());
		}

		void setVertexAttrib2f(Kint location, const KVector::vec3& v)const {
			glVertexAttrib2fv(location, v.data());
		}

		void bind()const {
			glBindVertexArray(array_id);
		}

		void unBind()const {
			glBindVertexArray(0);
		}

		void clearLocation() {
			delete locations;
			locations = nullptr;
		}

		void enableVertexArray()const {
			if (locations == nullptr) return;
			for (auto loc : *locations) {
				glEnableVertexAttribArray(loc);
			}
		}

		void disableVertexArray()const {
			if (locations == nullptr) return;
			for (auto loc : *locations) {
				glDisableVertexAttribArray(loc);
			}
		}
	};
} }

#endif // !KENGINES_VERTEX_ARRAY_H

