#pragma once

namespace Engine {


	struct Collider : public Component {

		struct Collider(){}

		virtual glm::vec3 FarthestPoint(const glm::vec3 &dir) const = 0;

	};


}