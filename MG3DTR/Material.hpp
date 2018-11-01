#pragma once

namespace Engine {
	struct Material {
		glm::vec3 ambient;
		glm::vec3 specular;
		glm::vec3 diffuse;
		float shininess;

		Material() :
			ambient(glm::vec3(0, 0, 0)),
			specular(glm::vec3(0, 0, 0)),
			diffuse(glm::vec3(0, 0, 0)),
			shininess(0.0f) {}

		Material(glm::vec3 ambient, glm::vec3 specular, glm::vec3 diffuse, float shininess) :
			ambient(ambient),
			specular(specular),
			diffuse(diffuse),
			shininess(shininess) {}
	};
}