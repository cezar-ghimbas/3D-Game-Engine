#pragma once

namespace Engine {

	struct Object {
		Transform* transform;
		MeshRenderer renderer;
		Material material;
		unsigned int texture;
		std::vector<Vertex> verts;

		Object(const std::string& vertFile, const std::string& textureFile) {
			ReadOBJ(vertFile, verts, material);
			texture = LoadTexture(textureFile);
			transform = new Transform();
		}
	};
}