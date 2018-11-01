#pragma once

namespace Engine {
	class RayCaster {

	public:
		RayCaster(){}

		void SetWindowSize(const glm::vec2 &ws)
		{
			windowSize = ws;
		}

		glm::vec3 GetRayDir(const glm::vec2 &screen_coords, const glm::mat4 &proj_m, const glm::mat4 &view_m)
		{
			glm::vec2 norm_device_coords = CalcNormDeviceCoords(screen_coords);
			glm::vec4 res_coords = glm::vec4(norm_device_coords, -1.0f, 1.0f);
			res_coords = glm::inverse(proj_m) * res_coords;
			res_coords = glm::vec4(res_coords.x, res_coords.y, -1.0f, 0.0f);
			res_coords = glm::inverse(view_m) * res_coords;

			rayDir = glm::normalize(glm::vec3(res_coords));

			return rayDir;
		}

		glm::vec3 GetRayDir() const
		{
			return rayDir;
		}

		~RayCaster(){}
	private:
		glm::vec3 rayDir;
		glm::vec2 windowSize;

		glm::vec2 CalcNormDeviceCoords(const glm::vec2 &screen_coords) const
		{
			return glm::vec2(2.0f * screen_coords.x / windowSize.x - 1.0f, 
				             -(2.0f * screen_coords.y / windowSize.y - 1.0f));
		}

	};
}