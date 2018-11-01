#pragma once
#include <string>
#include <vector>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "dependences/stb_image/stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "dependences/tinyobjloader/tiny_obj_loader.h"

namespace Engine {

	void ReadOBJ(const std::string& inFile, std::vector<Vertex>& verts, Material& mat) {
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inFile.c_str());

		glm::vec3 ambient = glm::vec3(materials[0].ambient[0], materials[0].ambient[1], materials[0].ambient[2]);
		glm::vec3 specular = glm::vec3(materials[0].specular[0], materials[0].specular[1], materials[0].specular[2]);
		glm::vec3 diffuse = glm::vec3(materials[0].diffuse[0], materials[0].diffuse[1], materials[0].diffuse[2]);
		mat = Material(ambient, specular, diffuse, materials[0].shininess);

		if (!err.empty()) {
			std::cerr << err << std::endl;
		}

		for (size_t s = 0; s < shapes.size(); s++) {
			size_t index_offset = 0;

			unsigned int currentIndex = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				int numFaceVerts = shapes[s].mesh.num_face_vertices[f];

				for (size_t v = 0; v < numFaceVerts; v++) {

					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
					tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

					verts.push_back(Vertex(vx, vy, vz, nx, ny, nz, tx, ty));


				}
				index_offset += numFaceVerts;

			}
		}
	}

	unsigned int LoadTexture(const std::string& inFile) {
		int width, height, bpp;
		unsigned int texture;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(inFile.c_str(), &width, &height, &bpp, 3);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		return texture;
	}


	void SendVertsToGPU(const std::vector<Vertex>& verts, unsigned int& vao, unsigned int& count) {
		unsigned int vbo, ibo;

		std::vector<unsigned int> inds;
		for (unsigned int i = 0; i < verts.size(); i++) {
			inds.push_back(i);
		}
		count = inds.size();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), &inds[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));
	}
}