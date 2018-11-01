#pragma once

#define KERNEL_SIZE		64
#define NOISE_SIZE		16

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

namespace Engine {

	struct ShadowMapRenderer {
		//std::vector<Engine::GameObject*> gameObjectVector;
		Test::BaseTest* scene;

		GLuint depthFramebuffer, quadVAO, quadIndexCount;
		GLuint shadowTexture;

		Engine::Shader /**shader,*/ *shader_quad, *shaderSsao, *shaderBlurSsao;
		Engine::Camera* camera;
		Engine::Light* light;

		Engine::Shader* shaderShadowMap;
		Engine::Shader* shaderColorsAndShadows;

		MeshRenderer screenQuad;

		glm::vec3* kernel;

		int drawType;/////////////////////////////////////////////////////////////////
		unsigned int material_shininess;/////////////////////////////////////////////
		float material_kd;///////////////////////////////////////////////////////////
		float material_ks;////////////////////////////////////////////////////////////

		ShadowMapRenderer(Test::BaseTest* scene) :
			scene(scene)
		{
			camera = GetFirstComponentOfType<Engine::Camera>();
			light = GetFirstComponentOfType<Engine::Light>();

			//camera->gameObject->scene->shader = new Engine::Shader("shaders/Phong/phong.vert", "shaders/Phong/phong.frag");

			scene->shader = new Engine::Shader("shaders/Phong/phong.vert", "shaders/Phong/phong.frag");
			shaderShadowMap = new Engine::Shader("shaders/ShadowMap/shadow_map_vertex.glsl", "shaders/ShadowMap/shadow_map_fragment.glsl");
			shaderColorsAndShadows = new Engine::Shader("shaders/ShadowMap/colors_and_shadows_vertex.glsl",
				"shaders/ShadowMap/colors_and_shadows_fragment.glsl");

			//screenQuad = LoadQuad();

			InitFramebuffer();

			drawType = 1;////////////////////////////////////////////////////////////////
			material_shininess = 10;////////////////////////////////////////////////////
			material_kd = 0.5;//////////////////////////////////////////////////////////
			material_ks = 0.5;//////////////////////////////////////////////////////////
		}

		~ShadowMapRenderer() {
			glDeleteTextures(1, &shadowTexture);
			glDeleteFramebuffers(1, &depthFramebuffer);
		}

		void Update() {
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_DEPTH_BUFFER_BIT);
			glUseProgram(shaderShadowMap->GetProgram());

			glCullFace(GL_FRONT);/////////////////

			glm::vec3 lightPos = light->gameObject->GetTransform()->position;
			glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
			glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0, -10, 0), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 lightSpaceMatrix = lightProjection * lightView;
			glUniformMatrix4fv(glGetUniformLocation(shaderShadowMap->GetProgram(), "lightSpaceMatrix"), 1, false, glm::value_ptr(lightSpaceMatrix));

			//Draw all objects//////////////////////////////////////
			scene->shader = shaderShadowMap;
			for (auto go : scene->gameObjectVector) {
				go->Update();
			}

			glCullFace(GL_BACK);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, camera->gameObject->scene->width, camera->gameObject->scene->height);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderColorsAndShadows->GetProgram());

			glBindTexture(GL_TEXTURE_2D, shadowTexture);

			glUniformMatrix4fv(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "lightSpaceMatrix"), 1, false, glm::value_ptr(lightSpaceMatrix));

			glm::vec3 cameraPos = camera->gameObject->GetTransform()->position;
			glUniform3f(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "light_position"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "eye_position"), cameraPos.x, cameraPos.y, cameraPos.z);
			glUniform1i(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "material_shininess"), material_shininess);
			glUniform1f(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "material_kd"), material_kd);
			glUniform1f(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "material_ks"), material_ks);
			glUniformMatrix4fv(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "view_matrix"), 1, false, glm::value_ptr(camera->GetViewMat()));
			glUniformMatrix4fv(glGetUniformLocation(shaderColorsAndShadows->GetProgram(), "projection_matrix"), 1, false, glm::value_ptr(camera->projection));


			//Draw all objects
			scene->shader = shaderColorsAndShadows;
			for (auto go : scene->gameObjectVector) {
				go->Update();
			}
		}


		MeshRenderer LoadQuad() {
			static const GLfloat vertices[] = {
				-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
				1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
				-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
				-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
				1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
				1.0f,  1.0f, 0.0f,   1.0f, 1.0f
			};

			static const GLuint inds[] = {
				0, 1, 2,
				3, 4, 5
			};

			GLuint quadVBO, quadIBO, quadVAO;
			GLuint quadIndexCount = 6;

			glGenVertexArrays(1, &quadVAO);
			glBindVertexArray(quadVAO);

			glGenBuffers(1, &quadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glGenBuffers(1, &quadIBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

			MeshRenderer res;
			res.vao = quadVAO;
			res.count = quadIndexCount;

			return res;
		}

		void InitFramebuffer() {

			depthFramebuffer = 0;
			glGenFramebuffers(1, &depthFramebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

			glGenTextures(1, &shadowTexture);
			glBindTexture(GL_TEXTURE_2D, shadowTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			//glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);


			/*GLuint rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_HEIGHT, WINDOW_WIDTH); // Use a single renderbuffer object for both a depth AND stencil buffer.
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it*/

			/*GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
			glDrawBuffers(1, drawBuffers);*/

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "FRAMEBUFFER ERROR\n";
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		template <typename T>
		T* GetFirstComponentOfType() {
			for (auto& go : scene->gameObjectVector) {
				T* component = go->GetComponent<T>();
				if (component != nullptr) {
					return component;
				}
			}
			return nullptr;
		}
	};
}