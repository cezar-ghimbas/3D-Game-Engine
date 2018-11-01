#pragma once

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	1000

#define OBJ_TRANS_VAL	0.1f
#define OBJ_ROT_VAL		0.1f
#define CAM_TRANS_VAL	2.0f
#define CAM_ROT_VAL		0.5f
#define COEF_REST		0.5f

#define MAX_ANG_VEL		3.0f
#define MIN_VEL			0.2f

#define FOV				120.0f
#define NEAR			0.1f
#define FAR				10000.0f

using namespace glm;



namespace Test {

	class TestSimple : public BaseTest {

	public:
		glm::vec3 rot;
		glm::vec3 scale;
		glm::vec3 translate;

		glm::vec2 mouseClickPos;

		Engine::RayCaster* rayCaster;

		unsigned int c_obj;
		Engine::Camera *cam;
		Engine::Camera* fpsCam;
		Engine::Camera* tpsCam;

		Engine::GameObject* root;
		Engine::GameObject* gobj2;
		Engine::GameObject* gobj3;

		int xCursor, yCursor;

		std::vector<Engine::Object*> objects;

		GLenum polyMode;

		glm::vec3 prevCoords, currentCoords, startCoords;

		unsigned int inFrustumCount;

		TestSimple()
		{
			Init();
		}

		~TestSimple() {
			for (auto& el : objects) {
				glDeleteTextures(1, &el->texture);
			}

		}
		void Init()
		{
			glClearColor(0.5, 0.5, 0.5, 1);
			glClearDepth(1);
			glEnable(GL_DEPTH_TEST);

			polyMode = GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, polyMode);

			shader = new Engine::Shader("shaders/Phong/phong.vert", "shaders/Phong/phong.frag");
			rayCaster = new Engine::RayCaster();

			mouseClickPos = glm::vec2(0, 0);

			rot = glm::vec3();
			scale = glm::vec3(1, 1, 1);
			translate = glm::vec3();

			input = new Engine::Input();

			
			gameObjectVector = Engine::CreateScene("resources/scenes/scene_10.xml", this);
			cam = GetFirstComponentWithType<Engine::Camera>();

		}

		template <typename T>
		T* GetFirstComponentWithType() {
			for (auto& go : gameObjectVector) {
				T* component = go->GetComponent<T>();
				if (component != nullptr) {
					return component;
				}
			}
			return nullptr;
		}


		void UpdateObject(Engine::GameObject* obj, Engine::Shader* shader) {
			obj->Update();
		}

		void DoFrustumCulling() {
			inFrustumCount = 0;
			for (auto& go : gameObjectVector) {
				auto obb = go->GetComponent<Engine::OrientedBoundingBox>();
				if (obb == nullptr) {
					continue;
				}
				auto meshRenderer = go->GetComponent<Engine::MeshRenderer>();
				if (cam->IsInFrustum(go->GetComponent<Engine::OrientedBoundingBox>()->GetOBBPts())) {
					meshRenderer->enabled = true;
					inFrustumCount++;
				}
				else {
					meshRenderer->enabled = false;
				}
			}
		}

		void Update() {
			ProcessInput();
			DoFrustumCulling();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			for (auto& go : gameObjectVector) {
				go->Update();
			}

			for (int i = 0; i < gameObjectVector.size(); i++) {
				for (int j = i + 1; j < gameObjectVector.size(); j++) {
					if (gameObjectVector[i]->GetComponent<Engine::Collider>() != nullptr &&
						gameObjectVector[j]->GetComponent<Engine::Collider>() != nullptr) {

						bool areColliding = Engine::GJK(gameObjectVector[i]->GetComponent<Engine::Collider>(),
							gameObjectVector[j]->GetComponent<Engine::Collider>());
						std::cout << i << " " << j << " " << std::boolalpha << areColliding << "\n";
					}
				}
			}
			std::cout << "\n";

			glFlush();
		}

		void ProcessInput() {
			if (input->GetKey(GLFW_KEY_W)) {
				cam->TranslateForward(CAM_TRANS_VAL);
			}

			if (input->GetKey(GLFW_KEY_A)) {
				cam->TranslateRight(-CAM_TRANS_VAL);
			}

			if (input->GetKey(GLFW_KEY_S)) {
				cam->TranslateForward(-CAM_TRANS_VAL);
			}

			if (input->GetKey(GLFW_KEY_D)) {
				cam->TranslateRight(CAM_TRANS_VAL);
			}

			if (input->GetKey(GLFW_KEY_Q)) {
				cam->RotateOy(glm::radians(CAM_ROT_VAL));
			}

			if (input->GetKey(GLFW_KEY_E)) {
				cam->RotateOy(glm::radians(-CAM_ROT_VAL));
			}

			if (input->GetKey(GLFW_KEY_R)) {
				cam->TranslateUp(CAM_TRANS_VAL);
			}

			if (input->GetKey(GLFW_KEY_F)) {
				cam->TranslateUp(-CAM_TRANS_VAL);
			}


			if (input->GetKey(GLFW_KEY_1)) {
			}
			if (input->GetKey(GLFW_KEY_2)) {
			}

			if (input->GetKey(GLFW_KEY_3)) {
			}
			if (input->GetKey(GLFW_KEY_4)) {
			}

			if (input->GetKey(GLFW_KEY_F1)) {
				if (polyMode == GL_LINE)
					polyMode = GL_FILL;
				else
					polyMode = GL_LINE;
				glPolygonMode(GL_FRONT_AND_BACK, polyMode);
			}

			if (input->GetKey(GLFW_KEY_LEFT_ALT) && input->GetKey(GLFW_MOUSE_BUTTON_LEFT)) {
				currentCoords = rayCaster->GetRayDir(glm::vec2(xCursor, yCursor), cam->projection, cam->GetViewMat());
				glm::vec3 delta_coords = currentCoords - prevCoords;
				prevCoords = currentCoords;
				cam->RotateOy(glm::radians(0.5f * (mouseClickPos.x - xCursor)));
				cam->RotateOx(glm::radians(0.5f * (mouseClickPos.y - yCursor)));
				mouseClickPos = glm::vec2(xCursor, yCursor);
			}
		}

		void ReshapeFunc(GLFWwindow *w, int width, int height)
		{
			if (height == 0)
				height = 1;
			float aspect = (float)width / (float)height;
			glViewport(0, 0, width, height);
			this->width = width;
			this->height = height;
			cam->SetAspectRatio(aspect);
			rayCaster->SetWindowSize(glm::vec2(width, height));
		}

		void KeyFunc(GLFWwindow *w, int key, int scan, int act, int mods)
		{
			input->ProcessKey(key, act);
		}

		void CursorFunc(GLFWwindow* window, double x, double y)
		{
			xCursor = x;
			yCursor = y;
		}

		void MouseButtonFunc(GLFWwindow* window, int b, int act, int mods)
		{

			input->ProcessKey(b, act);
			if (input->GetKey(GLFW_MOUSE_BUTTON_LEFT)) {
				double x;
				double y;
				glfwGetCursorPos(window, &x, &y);
				mouseClickPos = glm::vec2(x, y);
			}
		}


		void ScrollFunc(GLFWwindow *window, double x_off, double y_off) {
			cam->TranslateForward(y_off);
		}
	};
}