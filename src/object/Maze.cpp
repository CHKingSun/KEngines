#include "Maze.h"
#include "../math/transform.h"
#include "../material/Material.h"
#include "../buffer/VertexArray.h"
#include "../render/Shader.h"

namespace KEngines { namespace KObject {
	const Kint Maze::A_LOCATION = 3;

	Maze::Maze(Ksize row /* = 20 */, Ksize col /* = 20 */, Kfloat box_size /* = 1.f */) :
		Object3D("Maze"), box_size(box_size), vertices(nullptr), tex_coords(nullptr),
		normals(nullptr), indices(nullptr), locations(nullptr) {
		generateBox();
		initBoxBuffers();
		reset(row, col);

		material = new KMaterial::Material(DARKGREY, GREY, GREY, 20.f);
		material->addTexture(new KMaterial::Texture(IMAGE_PATH + "stone.png", KMaterial::AMBIENT));
		material->addTexture(new KMaterial::Texture(IMAGE_PATH + "stone.png", KMaterial::DIFFUSE));
		material->addTexture(new KMaterial::Texture(IMAGE_PATH + "stone.png", KMaterial::SPECULAR));
	}

	Maze::~Maze() {
		delete vertices;
		delete tex_coords;
		delete normals;
		delete indices;
		delete locations;
	}

	void Maze::generateBox() {
		Kfloat x = box_size / 2.0f;
		Kfloat y = box_size / 2.0f;
		Kfloat z = box_size / 2.0f;
		vertices = new vec3[24]{
			vec3(-x, -y,  z), vec3( x, -y,  z), vec3( x,  y,  z), vec3(-x,  y,  z),
			vec3( x, -y,  z), vec3( x, -y, -z), vec3( x,  y, -z), vec3( x,  y,  z),
			vec3( x, -y, -z), vec3(-x, -y, -z), vec3(-x,  y, -z), vec3( x,  y, -z),
			vec3(-x, -y, -z), vec3(-x, -y,  z), vec3(-x,  y,  z), vec3(-x,  y, -z),
			vec3(-x,  y,  z), vec3( x,  y,  z), vec3( x,  y, -z), vec3(-x,  y, -z),
			vec3( x, -y, -z), vec3(-x, -y, -z), vec3(-x, -y,  z), vec3( x, -y,  z)
		};

		Kfloat max_num = x > y ? x : y;
		max_num = max_num > z ? max_num : z;
		Kfloat tx = x / max_num;
		Kfloat ty = y / max_num;
		Kfloat tz = z / max_num;
		tex_coords = new vec2[24]{
			vec2(0, 0), vec2(tx, 0), vec2(tx, ty), vec2(0, ty),
			vec2(0, 0), vec2(tz, 0), vec2(tz, ty), vec2(0, ty),
			vec2(0, 0), vec2(tx, 0), vec2(tx, ty), vec2(0, ty),
			vec2(0, 0), vec2(tz, 0), vec2(tz, ty), vec2(0, ty),
			vec2(0, 0), vec2(tx, 0), vec2(tx, tz), vec2(0, tz),
			vec2(0, 0), vec2(tx, 0), vec2(tx, tz), vec2(0, tz),
		};

		normals = new vec3[24]{
			vec3( 0,  0,  1), vec3( 0,  0,  1), vec3( 0,  0,  1), vec3( 0,  0,  1),
			vec3( 1,  0,  0), vec3( 1,  0,  0), vec3( 1,  0,  0), vec3( 1,  0,  0),
			vec3( 0,  0, -1), vec3( 0,  0, -1), vec3( 0,  0, -1), vec3( 0,  0, -1),
			vec3(-1,  0,  0), vec3(-1,  0,  0), vec3(-1,  0,  0), vec3(-1,  0,  0),
			vec3( 0,  1,  0), vec3( 0,  1,  0), vec3( 0,  1,  0), vec3( 0,  1,  0),
			vec3( 0, -1,  0), vec3( 0, -1,  0), vec3( 0, -1,  0), vec3( 0, -1,  0)
		};

		indices = new Kubyte[36]{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};
	}

	void Maze::initBoxBuffers() {
		vao = new KBuffer::VertexArray();

		vbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec3) * 24, vertices);
		vao->allocate(vbo, A_POSITION, 3);

		tbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec2) * 24, tex_coords);
		vao->allocate(tbo, A_TEX_COORD, 2);

		nbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, sizeof(vec3) * 24, normals);
		vao->allocate(nbo, A_NORMAL, 3);

		ibo = new KBuffer::VertexBuffer(KBuffer::INDEX, sizeof(Kubyte) * 36, indices);

		delete[] vertices; vertices = nullptr;
		delete[] tex_coords; tex_coords = nullptr;
		delete[] normals; normals = nullptr;
		delete[] indices; indices = nullptr;
	}

	Ksize Maze::initMaze() {
		srand(10);

		maze.clear();
		maze.resize(row + 2, std::vector<bool>(col + 2, false));

		entry = rand() % row + 1;
		exit = -1;
		maze[entry][0] = true;
		return (row + 2) * (col + 2) - 1 - traverse(entry, 1);
	}

	Ksize Maze::traverse(Ksize m, Ksize n) {
		maze[m][n] = true;

		Direction directions[] = { D_UP, D_DOWN, D_LEFT, D_RIGHT };
		for (int i = 0; i < 4; i++) { //for random direction.
			int j = rand() % 4;
			if (i == j) {
				i--; continue;
			}
			Direction t = directions[i];
			directions[i] = directions[j];
			directions[j] = t;
		}

		unsigned int count = 1;
		for (auto direction : directions) {
			switch (direction) {
			case D_UP:
				if (m > 1 && !maze[m - 1][n] && isUpBlock(m - 1, n)
					&& isLeftBlock(m - 1, n) && isRightBlock(m - 1, n))
					count += traverse(m - 1, n);
				break;
			case D_DOWN:
				if (m < row && !maze[m + 1][n] && isDownBlock(m + 1, n)
					&& isLeftBlock(m + 1, n) && isRightBlock(m + 1, n))
					count += traverse(m + 1, n);
				break;
			case D_LEFT:
				if (n > 1 && !maze[m][n - 1] && isLeftBlock(m, n - 1)
					&& isUpBlock(m, n - 1) && isDownBlock(m, n - 1))
					count += traverse(m, n - 1);
				break;
			case D_RIGHT:
				if (n < col && !maze[m][n + 1] && isRightBlock(m, n + 1)
					&& isUpBlock(m, n + 1) && isDownBlock(m, n + 1))
					count += traverse(m, n + 1);
				else if (n == col && exit == -1) {
					exit = m;
					maze[exit][col + 1] = true;
				}
				break;
			}
		}
		return count;
	}

	void Maze::bindMatrices() {
		if (locations == nullptr) return;
		delete mbo;

		mbo = new KBuffer::VertexBuffer(KBuffer::VERTEX, locations->size() * sizeof(vec3),
			locations->data());
		vao->allocate(mbo, A_LOCATION, 3);
		glVertexAttribDivisor(A_LOCATION, 1);

		delete locations; locations = nullptr;
	}

	void Maze::reset(Ksize row /* = 20 */, Ksize col /* = 20 */) {
		this->row = row;
		this->col = col;
		count = (row + 2) * (col + 2) + initMaze();

		delete locations;
		locations = new std::vector<vec3>();
		locations->reserve(count);

		const Kfloat per_offset = box_size;
		const Kfloat x_offset = -per_offset * (row + 1) / 2;
		const Kfloat z_offset = -per_offset * (col + 1) / 2;

		for (int z = 0; z < row + 2; ++z) {
			for (int x = 0; x < col + 2; ++x) {
				locations->emplace_back(
					x_offset + per_offset * x, -per_offset / 2, z_offset + per_offset * z
				);
				if (!maze[x][z]) {
					locations->emplace_back(
						x_offset + per_offset * x, per_offset / 2, z_offset + per_offset * z
					);
				}
			}
		}
		bindMatrices();
	}

	void Maze::render(const KRenderer::Shader* shader)const {
		bind();

		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr, count);

		unBind();
		if (material != nullptr) material->unActiveTextures(shader);
	}
} }