#pragma once

namespace Engine {
	struct Debug {
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const Camera* cam) {
			Engine::Shader shader = Engine::Shader("shaders/DrawLine/draw_line.vert", "shaders/DrawLine/draw_line.geom", "shaders/DrawLine/draw_line.frag");
			glUseProgram(shader.GetProgram());
			glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "proj_mat"), 1, false, glm::value_ptr(cam->projection));
			glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view_mat"), 1, false, glm::value_ptr(cam->GetViewMat()));
			glUniform3f(glGetUniformLocation(shader.GetProgram(), "startPoint"), start.x, start.y, start.z);
			glUniform3f(glGetUniformLocation(shader.GetProgram(), "endPoint"), end.x, end.y, end.z);
			glDrawArrays(GL_POINTS, 0, 1);
			glDeleteProgram(shader.GetProgram());
		}

		void DrawPoint(const glm::vec3& pt, const Camera* cam) {
			Engine::Shader shader = Engine::Shader("shaders/DrawPoint/draw_point.vert", "shaders/DrawPoint/draw_point.frag");
			glUseProgram(shader.GetProgram());
			glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "proj_mat"), 1, false, glm::value_ptr(cam->projection));
			glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view_mat"), 1, false, glm::value_ptr(cam->GetViewMat()));
			glUniform3f(glGetUniformLocation(shader.GetProgram(), "point"), pt.x, pt.y, pt.z);
			glEnable(GL_PROGRAM_POINT_SIZE);
			glDrawArrays(GL_POINTS, 0, 1);
		}

		static void DrawBoundingBox(const std::vector<glm::vec3>& pts, const Camera* cam) {
			DrawLine(pts[0], pts[1], cam);
			DrawLine(pts[1], pts[2], cam);
			DrawLine(pts[2], pts[3], cam);
			DrawLine(pts[3], pts[0], cam);

			DrawLine(pts[4], pts[5], cam);
			DrawLine(pts[5], pts[6], cam);
			DrawLine(pts[6], pts[7], cam);
			DrawLine(pts[7], pts[4], cam);

			DrawLine(pts[0], pts[4], cam);
			DrawLine(pts[1], pts[5], cam);
			DrawLine(pts[2], pts[6], cam);
			DrawLine(pts[3], pts[7], cam);
		}
	};
}