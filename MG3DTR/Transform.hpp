#pragma once

namespace Engine {

	struct Transform : public Component {
	public:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		Transform() :
			position(glm::vec3(0, 0, 0)),
			rotation(glm::vec3(0, 0, 0)),
			scale(glm::vec3(1, 1, 1)) {}

		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
			position(position),
			rotation(rotation),
			scale(scale) {}

		void Translate(const glm::vec3& delta) {
			position += delta;
		}

		void Rotate(const glm::vec3& eulerAngles) {
			rotation = glm::quat(eulerAngles);
		}

		void Scale(glm::vec3 scale) {
			this->scale = scale;
		}

		glm::mat4 GetModelMatrix() {
			glm::mat4 modelMat = glm::mat4(1.0f);
			for (auto* go = gameObject; go != nullptr; go = go->parent) {
				modelMat = glm::scale(glm::mat4(), go->GetTransform()->scale) * modelMat;
				modelMat = glm::toMat4(go->GetTransform()->rotation) * modelMat;
				modelMat = glm::translate(glm::mat4(), go->GetTransform()->position) * modelMat;
			}
			return modelMat;
		}

		void Update() override {
			unsigned int shaderProgram = gameObject->scene->shader->GetProgram();
			glUseProgram(shaderProgram);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelMatrix"), 1, false, glm::value_ptr(GetModelMatrix()));
		}
	};
}