#pragma once
#include <vector>

namespace Engine {

	struct MeshRenderer : public Component {

		unsigned int vao;
		unsigned int count;
		Shader* shader;
		unsigned int texture;
		Material material;

		MeshRenderer() : vao(0), count(0) {}
		MeshRenderer(unsigned int vao, unsigned int count, Shader* shader, unsigned int texture, Material material) :
			vao(vao),
			count(count),
			shader(shader),
			texture(texture),
			material(material) {}

		void SetShader(Shader* shader) {
			this->shader = shader;
		}

		void Update() override {
			unsigned int shaderProgram = gameObject->scene->shader->GetProgram();
			glUseProgram(shaderProgram);

			glUniform3f(glGetUniformLocation(shaderProgram, "ambient"), material.ambient.x, material.ambient.y, material.ambient.z);
			glUniform3f(glGetUniformLocation(shaderProgram, "specular"), material.specular.x, material.specular.y, material.specular.z);
			glUniform3f(glGetUniformLocation(shaderProgram, "diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
			glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), material.shininess);

			glUniform1i(glGetUniformLocation(shaderProgram, "objTexture"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);

			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		}
	};
}