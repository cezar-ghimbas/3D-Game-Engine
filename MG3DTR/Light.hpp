#pragma once

namespace Engine {
	struct Light : public Component {
		glm::vec3 color;

		Light(glm::vec3 color) : color(color) {}

		void Update() override {
			glm::vec3 position = gameObject->GetTransform()->position;
			unsigned int shaderProgram = gameObject->scene->shader->GetProgram();
			glUseProgram(shaderProgram);
			glUniform3f(glGetUniformLocation(shaderProgram, "lightPosition"), position.x, position.y, position.z);
			glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), color.x, color.y, color.z);
		}

	};
}