#pragma once

namespace Engine {
	struct AxisAlignedBoundingBox : Component {
	private:
		glm::vec3 localBottomLeft, localTopRight;
	public:
		glm::vec3 bottomLeft, topRight;

		AxisAlignedBoundingBox(const glm::vec3& bottomLeft, const glm::vec3& topRight) : 
			localBottomLeft(bottomLeft),
			localTopRight(topRight) {}

		void Update() override {
			glm::mat4 modelMatrix = gameObject->GetTransform()->GetModelMatrix();
			std::vector<glm::vec3> transformedPts = GetTransformedPts();

			bottomLeft = glm::vec3(FLT_MAX, FLT_MAX, -FLT_MAX);
			topRight = glm::vec3(-FLT_MAX, -FLT_MAX, FLT_MAX);
			for (auto& pt : transformedPts) {
				bottomLeft.x = bottomLeft.x > pt.x ? pt.x : bottomLeft.x;
				bottomLeft.y = bottomLeft.y > pt.y ? pt.y : bottomLeft.y;
				bottomLeft.z = bottomLeft.z < pt.z ? pt.z : bottomLeft.z;

				topRight.x = topRight.x < pt.x ? pt.x : topRight.x;
				topRight.y = topRight.y < pt.y ? pt.y : topRight.y;
				topRight.z = topRight.z > pt.z ? pt.z : topRight.z;
			}
		}

		std::vector<glm::vec3> GetTransformedPts() {
			glm::mat4 modelMatrix = gameObject->GetTransform()->GetModelMatrix();
			std::vector<glm::vec3> transformedPts;

			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localBottomLeft, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localTopRight.x, localBottomLeft.y, localBottomLeft.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localTopRight.x, localBottomLeft.y, localTopRight.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localBottomLeft.x, localBottomLeft.y, localTopRight.z, 1.0f)));

			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localBottomLeft.x, localTopRight.y, localBottomLeft.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localTopRight.x, localTopRight.y, localBottomLeft.z, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localTopRight, 1.0f)));
			transformedPts.push_back(glm::vec3(modelMatrix * glm::vec4(localBottomLeft.x, localTopRight.y, localTopRight.z, 1.0f)));

			return transformedPts;
		}

		std::vector<glm::vec3> GetAABBPts() {
			std::vector<glm::vec3> pts;
			pts.push_back(glm::vec3(bottomLeft));
			pts.push_back(glm::vec3(topRight.x, bottomLeft.y, bottomLeft.z));
			pts.push_back(glm::vec3(topRight.x, bottomLeft.y, topRight.z));
			pts.push_back(glm::vec3(bottomLeft.x, bottomLeft.y, topRight.z));

			pts.push_back(glm::vec3(bottomLeft.x, topRight.y, bottomLeft.z));
			pts.push_back(glm::vec3(topRight.x, topRight.y, bottomLeft.z));
			pts.push_back(glm::vec3(topRight));
			pts.push_back(glm::vec3(bottomLeft.x, topRight.y, topRight.z));

			return pts;
		}

		static void GetAABBLimits(const std::vector<Engine::Vertex>& verts, glm::vec3& bottomLeft, glm::vec3& topRight) {
			for (auto& pt : verts) {
				bottomLeft.x = bottomLeft.x > pt.x ? pt.x : bottomLeft.x;
				bottomLeft.y = bottomLeft.y > pt.y ? pt.y : bottomLeft.y;
				bottomLeft.z = bottomLeft.z < pt.z ? pt.z : bottomLeft.z;

				topRight.x = topRight.x < pt.x ? pt.x : topRight.x;
				topRight.y = topRight.y < pt.y ? pt.y : topRight.y;
				topRight.z = topRight.z > pt.z ? pt.z : topRight.z;
			}
		}
	};
}