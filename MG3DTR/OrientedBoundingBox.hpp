#pragma once

namespace Engine {
	struct OrientedBoundingBox : Component {

		glm::vec3 bottomLeft, topRight;

		OrientedBoundingBox(const glm::vec3& bottomLeft, const glm::vec3& topRight) :
			bottomLeft(bottomLeft),
			topRight(topRight) {}

		
		std::vector<glm::vec3> GetOBBPts() {
			glm::mat4 modelMatrix = gameObject->GetTransform()->GetModelMatrix();
			std::vector<glm::vec3> transformedPts;

			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(bottomLeft, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(topRight.x, bottomLeft.y, bottomLeft.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(topRight.x, bottomLeft.y, topRight.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(bottomLeft.x, bottomLeft.y, topRight.z, 1.0f)));

			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(bottomLeft.x, topRight.y, bottomLeft.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(topRight.x, topRight.y, bottomLeft.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(topRight, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(bottomLeft.x, topRight.y, topRight.z, 1.0f)));

			return transformedPts;
		}
	};
}