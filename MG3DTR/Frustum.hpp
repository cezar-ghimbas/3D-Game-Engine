#pragma once
#include <vector>
#include <iostream>

namespace Engine {
	struct Frustum {
	private:
		struct Plane {
			glm::vec3 normal;
			glm::vec3 point;

			Plane() {}
			Plane(const glm::vec3& normal, const glm::vec3& point) : 
				normal(normal),
				point(point)
			{}
		};

	public:
		Plane nearPlane, farPlane, leftPlane, rightPlane, topPlane, bottomPlane;

		glm::vec3 dirInRightPlane;
		glm::vec3 dirInTopPlane;
		glm::vec3 dirInLeftPlane;
		glm::vec3 dirInBottomPlane;

		Frustum() {}

		Frustum(glm::vec3 cameraPosition, glm::vec3 cameraDirection, glm::vec3 up, float fov, float ratio, float near, float far) {
			float heightNear = 2 * glm::tan(fov / 2.0f) * near;
			float widthNear = ratio * heightNear;
			float heightFar = 2 * glm::tan(fov / 2.0f) * far;
			float widthFar = ratio * heightFar;

			up = glm::normalize(up);
			cameraDirection = glm::normalize(cameraDirection);
			glm::vec3 right = glm::normalize(glm::cross(cameraDirection, up));


			glm::vec3 farCenter = cameraPosition + cameraDirection * far;
			glm::vec3 nearCenter = cameraPosition + cameraDirection * near;
			dirInRightPlane = glm::normalize((nearCenter + right * widthNear / 2.0f) - cameraPosition);
			dirInTopPlane = glm::normalize((nearCenter + up * heightNear / 2.0f) - cameraPosition);
			dirInLeftPlane = glm::normalize((nearCenter - right * widthNear / 2.0f) - cameraPosition);
			dirInBottomPlane = glm::normalize((nearCenter - up * heightNear / 2.0f) - cameraPosition);

			nearPlane = Plane(cameraDirection, nearCenter);
			farPlane = Plane(-cameraDirection, farCenter);
			rightPlane = Plane(glm::cross(up, dirInRightPlane), cameraPosition);
			topPlane = Plane(glm::cross(-right, dirInTopPlane), cameraPosition);
			leftPlane = Plane(glm::cross(dirInLeftPlane, up), cameraPosition);
			bottomPlane = Plane(glm::cross(dirInBottomPlane, -right), cameraPosition);
		}

		bool IsInFrustum(const std::vector<glm::vec3>& pts) {
			std::vector<Plane> planes = {nearPlane, farPlane, leftPlane, rightPlane, topPlane, bottomPlane};
			for (auto& plane : planes) {
				int sameSidePlaneNormal = 0;
				int outsidePlaneNormal = 0;
				for (auto& pt : pts) {
					float signedDist = glm::dot(plane.normal, pt - plane.point);
					if (signedDist < 0) {
						outsidePlaneNormal++;
					}
					else {
						sameSidePlaneNormal++;
					}
				}
				if (sameSidePlaneNormal == 0) {
					return false;
				}
			}

			return true;
		}
	};
}