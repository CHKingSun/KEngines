//
// Created by KingSun on 2018/09/02
//

#ifndef KENGINES_MAZE_H
#define KENGINES_MAZE_H

#include <vector>

#include "Object3D.h"

namespace KEngines { namespace KBuffer { class VertexBuffer; } }

namespace KEngines { namespace KObject {
	enum Direction {
		D_UP = 0, D_DOWN, D_LEFT, D_RIGHT
	};

	class Maze : public Object3D {
	private:
		Ksize row, col;
		Ksize count;
		Kfloat box_size;

		vec3* vertices;
		vec2* tex_coords;
		vec3* normals;
		Kubyte* indices;
		std::vector<vec3>* locations;

		KBuffer::VertexBuffer* mbo;

		Ksize entry, exit;
		std::vector<std::vector<Kboolean>> maze;

		static const Kint A_LOCATION;

		void generateBox();

		Ksize initMaze();

		Ksize traverse(Ksize m, Ksize n);

		void initBoxBuffers();

		void bindMatrices();

		inline Kboolean isUpBlock(int m, int n)const {
			return (m == 1) || !maze[m - 1][n];
		}

		inline Kboolean isDownBlock(int m, int n)const {
			return (m == row) || !maze[m + 1][n];
		}

		inline Kboolean isLeftBlock(int m, int n)const {
			return (n == 1) || !maze[m][n - 1];
		}

		inline Kboolean isRightBlock(int m, int n)const {
			return (n == col) || !maze[m][n + 1];
		}

	public:
		Maze(Ksize row = 20, Ksize col = 20, Kfloat box_size = 1.f);

		~Maze();

		void reset(Ksize row = 20, Ksize col = 20);

		void render(const KRenderer::Shader* shader /* = nullptr */)const override;

		vec3 getStartPosition()const {
			return vec3(
				box_size * (2 * entry - row - 1) / 2, box_size / 2, -box_size * (col + 1) / 2
			);
		}
	};
} }

#endif // !KENGINES_MAZE_H
