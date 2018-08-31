//
// Created by KingSun on 2018/08/15
//

#ifndef KENGINES_CUBE_TEXTURE_H
#define KENGINES_CUBE_TEXTURE_H

#include "../KHeader.h"
#include "../math/matrix/mat4.h"
#include <vector>

namespace KEngines {
	namespace KRenderer { class Shader; }
	namespace KBuffer {
		class VertexArray;
		class VertexBuffer;
	}
	namespace KCamera { class CameraInterface; }
}

namespace KEngines { namespace KMaterial {

	class CubeTexture {
	private:
		Kuint tex_id;
		Kuint tex_unit;

		std::vector<std::string> paths;
		static const std::string U_CUBE_TEX;

		KRenderer::Shader* map_shader;
		KBuffer::VertexArray* vao;
		KBuffer::VertexBuffer* vbo;
		KBuffer::VertexBuffer* ibo;
		
		void initGL();

	public:
		//Texture order: right, left, up, down, back, front
		CubeTexture(const std::vector<std::string>& paths, Kuint tex_unit = 0);
		~CubeTexture();

		static Kuint getCubeTextrueId(const std::vector<std::string>& paths);

		void setCubeTextureUnit(Kuint tex_unit) { this->tex_unit = tex_unit; }

		void bindUniform(const KRenderer::Shader* shader)const;

		void bindMatrix(const KCamera::CameraInterface* camera)const;

		void unActive(const KRenderer::Shader* shader = nullptr)const;

		void render()const;
	};
} }

#endif // !KENGINES_CUBE_TEXTURE_H

