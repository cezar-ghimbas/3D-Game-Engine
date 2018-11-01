#pragma once

namespace Engine {
	struct BoxCollider : public Collider {
		BoxCollider()
		{
		}

		glm::vec3 FarthestPoint(const glm::vec3 &dir) const override {
			float dot;
			auto verts = gameObject->GetComponent<OrientedBoundingBox>()->GetOBBPts();

			float max = glm::dot(verts[0], dir);
			glm::vec3 res = verts[0];
			unsigned int size = verts.size();

			for (unsigned int i = 1; i < size; i++) {
				dot = glm::dot(verts[i], dir);
				if (dot > max) {
					max = dot;
					res = verts[i];
				}
			}

			return res;
		}
	};
}