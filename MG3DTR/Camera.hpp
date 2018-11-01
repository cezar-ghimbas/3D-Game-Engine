namespace Engine {

	class Camera : public Component {
	protected:
		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;

		float aspectRatio;
		float near;
		float far;
		Frustum frustum;

	public:
		float fieldOfView;
		glm::mat4 projection;

		Camera(const glm::vec3& pos, const glm::vec3& center, const glm::vec3& u)
		{
			forward = glm::normalize(center - pos);
			up = u;
			right = glm::cross(forward, up);
		}

		Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up, float fieldOfView, float aspectRatio, float near, float far) :
			up(up),
			fieldOfView(fieldOfView),
			aspectRatio(aspectRatio),
			near(near),
			far(far) {

			forward = glm::normalize(center - position);
			right = glm::cross(forward, up);
			frustum = Frustum(position, forward, up, fieldOfView, aspectRatio, near, far);
		}

		void TranslateForward(const float& distance)
		{
			gameObject->GetTransform()->Translate(glm::normalize(forward) * distance);
			UpdateFrustum();
		}

		void TranslateRight(const float& distance)
		{
			gameObject->GetTransform()->Translate(glm::normalize(right) * distance);
			UpdateFrustum();
		}

		void TranslateUp(const float& distance)
		{
			gameObject->GetTransform()->Translate(glm::normalize(up) * distance);
			UpdateFrustum();
		}

		virtual void RotateOy(const float& angle)
		{
		}

		virtual void RotateOx(const float& angle)
		{
		}

		virtual void RotateOz(const float& angle)
		{
		}


		glm::mat4 GetViewMat() const
		{
			glm::vec3 position = gameObject->GetTransform()->position;
			glm::mat4 m = glm::lookAt(position, position + glm::normalize(forward), up);
			return m;
		}

		glm::vec3 GetForward() const
		{
			return glm::normalize(forward);
		}

		glm::vec3 GetUp() const
		{
			return glm::normalize(up);
		}

		float GetNear() const {
			return near;
		}

		float GetFar() const {
			return far;
		}

		void SetFieldOfView(float fieldOfView) {
			this->fieldOfView = fieldOfView;
			UpdateInternals();
		}

		void SetNear(float near) {
			this->near = near;
			UpdateInternals();
		}

		void SetFar(float far) {
			this->far = far;
			UpdateInternals();
		}

		void SetAspectRatio(float aspectRatio) {
			this->aspectRatio = aspectRatio;
			UpdateInternals();
		}

		bool IsInFrustum(const std::vector<glm::vec3>& pts) {
			return frustum.IsInFrustum(pts);
		}

		void Update() override {
			glm::vec3 position = gameObject->GetTransform()->position;
			unsigned int shaderProgram = gameObject->scene->shader->GetProgram();
			glUseProgram(shaderProgram);
			glUniform3f(glGetUniformLocation(shaderProgram, "eyePosition"), position.x, position.y, position.z);

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, false, glm::value_ptr(GetViewMat()));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projectionMatrix"), 1, false, glm::value_ptr(projection));
		}

		Camera() {}
		~Camera() {}

	protected:
		void UpdateFrustum() {
			glm::vec3 position = gameObject->GetTransform()->position;
			frustum = Frustum(position, forward, up, fieldOfView, aspectRatio, near, far);
		}

		void UpdateProjectionMatrix() {
			projection = glm::perspective(fieldOfView, aspectRatio, near, far);
		}

		void UpdateInternals() {
			glm::vec3 position = gameObject->GetTransform()->position;
			frustum = Frustum(position, forward, up, fieldOfView, aspectRatio, near, far);
			projection = glm::perspective(fieldOfView, aspectRatio, near, far);
		}
	};


	class FirstPersonCamera : public Camera {
	public:

		FirstPersonCamera(const glm::vec3& pos, const glm::vec3& center, const glm::vec3& u, float fieldOfView, float aspectRatio, float near, float far) :
			Camera(pos, center, u, fieldOfView, aspectRatio, near, far) {}

		void RotateOy(const float& angle) override
		{
			glm::quat fwdQuat = glm::quat(0, forward);
			glm::quat rotQuat = glm::angleAxis(angle, up);
			forward = glm::normalize(glm::vec3(glm::rotate(rotQuat, glm::vec4(forward, 1))));
			right = glm::normalize(glm::cross(forward, up));
			
			UpdateFrustum();
		}

		virtual void RotateOx(const float& angle) override
		{
			glm::quat fwdQuat = glm::quat(0, forward);
			glm::quat rotQuat = glm::angleAxis(angle, right);
			forward = glm::normalize(glm::vec3(glm::rotate(rotQuat, glm::vec4(forward, 1))));
			up = glm::normalize(glm::cross(right, forward));
			
			UpdateFrustum();
		}

		virtual void RotateOz(const float& angle) override
		{
			glm::quat rightQuat = glm::quat(0, right);
			glm::quat rotQuat = glm::angleAxis(angle, forward);
			right = glm::normalize(glm::vec3(glm::rotate(rotQuat, glm::vec4(right, 1))));
			up = glm::normalize(glm::cross(right, forward));
			
			UpdateFrustum();
		}

	};


	class ThirdPersonCamera : public Camera {
	private:
		glm::vec3 center;

	public:

		ThirdPersonCamera(const glm::vec3& pos, const glm::vec3& center, const glm::vec3& u) :
			Camera(pos, center, u), center(center) {}

		void RotateOy(const float& angle) override
		{
			glm::vec3 position = gameObject->GetTransform()->position;
			float dist = glm::distance(position, center);
			glm::quat rotQuat = glm::angleAxis(angle, up);
			glm::vec3 newForward = glm::normalize(glm::vec3(glm::rotate(rotQuat, glm::vec4(forward, 1))));
			right = glm::normalize(glm::cross(forward, up));
			position = position + dist * forward - dist * newForward;
			forward = newForward;

			UpdateFrustum();
		}

		virtual void RotateOx(const float& angle) override
		{
			glm::vec3 position = gameObject->GetTransform()->position;
			float dist = glm::distance(position, center);
			glm::quat rotQuat = glm::angleAxis(angle, right);
			glm::vec3 newForward = glm::normalize(glm::vec3(glm::rotate(rotQuat, glm::vec4(forward, 1))));
			up = glm::normalize(glm::cross(right, forward));
			position = position + dist * forward - dist * newForward;
			forward = newForward;

			UpdateFrustum();
		}

		virtual void RotateOz(const float& angle) override
		{
			glm::vec3 position = gameObject->GetTransform()->position;
			float dist = glm::distance(position, center);
			glm::quat rotQuat = glm::angleAxis(angle, forward);
			glm::vec3 newForward = glm::normalize(glm::vec3(glm::rotate(rotQuat, glm::vec4(forward, 1))));
			up = glm::normalize(glm::cross(right, forward));
			position = position + dist * forward - dist * newForward;
			forward = newForward;

			UpdateFrustum();
		}
	};


}