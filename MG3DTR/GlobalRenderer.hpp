#pragma once

#define KERNEL_SIZE		64
#define NOISE_SIZE		16



namespace Engine {

	struct GlobalRenderer {
		std::vector<Engine::GameObject*> gameObjectVector;

		GLuint framebuffer, framebufferSsao, framebufferBlur, posTexture;
		GLuint normTexture, colorTexture, depthTexture, rbo;
		GLuint ssaoTexture, blurTexture, noiseTexture;

		Engine::Shader /**shader,*/ *shader_quad, *shaderSsao, *shaderBlurSsao;
		Engine::Camera* camera;
		Engine::Light* light;

		MeshRenderer screenQuad;

		glm::vec3* kernel;

		int drawType;/////////////////////////////////////////////////////////////////
		unsigned int material_shininess;/////////////////////////////////////////////
		float material_kd;///////////////////////////////////////////////////////////
		float material_ks;////////////////////////////////////////////////////////////

		GlobalRenderer(std::vector<Engine::GameObject*> gameObjectVector) :
			gameObjectVector(gameObjectVector)
		{
			camera = GetFirstComponentOfType<Engine::Camera>();
			light = GetFirstComponentOfType<Engine::Light>();

			//camera->gameObject->scene->shader = new Engine::Shader("shaders/Phong/phong.vert", "shaders/Phong/phong.frag");
			shaderSsao = new Engine::Shader("shaders/ssao.vert", "shaders/ssao.frag");
			shaderBlurSsao = new Engine::Shader("shaders/ssao_blur.vert", "shaders/ssao_blur.frag");
			shader_quad = new Engine::Shader("shaders/screen_quad.vert", "shaders/screen_quad.frag");


			screenQuad = LoadQuad();

			InitFramebuffer();
			GenKernelHemisphere(kernel);
			InitNoiseTexture();

			drawType = 1;////////////////////////////////////////////////////////////////
			material_shininess = 10;////////////////////////////////////////////////////
			material_kd = 0.5;//////////////////////////////////////////////////////////
			material_ks = 0.5;//////////////////////////////////////////////////////////
		}

		~GlobalRenderer() {
			glDeleteTextures(1, &ssaoTexture);
			glDeleteTextures(1, &noiseTexture);
			glDeleteTextures(1, &posTexture);
			glDeleteTextures(1, &blurTexture);
			glDeleteTextures(1, &depthTexture);
			glDeleteTextures(1, &colorTexture);
			glDeleteTextures(1, &normTexture);
			glDeleteFramebuffers(1, &framebufferSsao);
			glDeleteFramebuffers(1, &framebufferBlur);
			glDeleteFramebuffers(1, &framebuffer);
		}

		void Update() {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			for (const auto& go : gameObjectVector) {
				go->Update();
			}

			if (camera == nullptr || light == nullptr) {
				return;
			}

			//SSAO TEXTURE
			glBindFramebuffer(GL_FRAMEBUFFER, framebufferSsao);
			glUseProgram(shaderSsao->GetProgram());

			glUniform1i(glGetUniformLocation(shaderSsao->GetProgram(), "posTexture"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, posTexture);

			glUniform1i(glGetUniformLocation(shaderSsao->GetProgram(), "noiseTexture"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);

			glUniform1i(glGetUniformLocation(shaderSsao->GetProgram(), "normTexture"), 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, normTexture);

			glUniform3fv(glGetUniformLocation(shaderSsao->GetProgram(), "sampleOffsets"), KERNEL_SIZE, (const GLfloat*)&kernel[0]);
			glUniformMatrix4fv(glGetUniformLocation(shaderSsao->GetProgram(), "V"), 1, false, glm::value_ptr(camera->GetViewMat()));
			glUniformMatrix4fv(glGetUniformLocation(shaderSsao->GetProgram(), "P"), 1, false, glm::value_ptr(camera->projection));
			glBindVertexArray(screenQuad.vao);
			glDisable(GL_DEPTH_TEST);
			glDrawElements(GL_TRIANGLES, screenQuad.count, GL_UNSIGNED_INT, 0);


			//SSAO TEXTURE BLUR
			glBindFramebuffer(GL_FRAMEBUFFER, framebufferBlur);
			glUseProgram(shaderBlurSsao->GetProgram());
			glUniform1i(glGetUniformLocation(shaderBlurSsao->GetProgram(), "ssaoTexture"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ssaoTexture);
			glBindVertexArray(screenQuad.vao);
			glDisable(GL_DEPTH_TEST);
			glDrawElements(GL_TRIANGLES, screenQuad.count, GL_UNSIGNED_INT, 0);


			//SCREEN
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glm::vec3 lightPos = light->gameObject->GetTransform()->position;
			glm::vec3 camPos = camera->gameObject->GetTransform()->position;
			glUseProgram(shader_quad->GetProgram());
			glUniform1f(glGetUniformLocation(shader_quad->GetProgram(), "nearPlane"), camera->GetNear());
			glUniform1f(glGetUniformLocation(shader_quad->GetProgram(), "farPlane"), camera->GetFar());
			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "drawType"), drawType);
			glUniform3f(glGetUniformLocation(shader_quad->GetProgram(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(shader_quad->GetProgram(), "eyePos"), camPos.x, camPos.y, camPos.z);
			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "materialShininess"), material_shininess);
			glUniform1f(glGetUniformLocation(shader_quad->GetProgram(), "materialKd"), material_kd);
			glUniform1f(glGetUniformLocation(shader_quad->GetProgram(), "materialKs"), material_ks);

			glBindVertexArray(screenQuad.vao);
			glDisable(GL_DEPTH_TEST);

			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "posTexture"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, posTexture);

			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "normTexture"), 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normTexture);

			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "colorTexture"), 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, colorTexture);

			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "depthTexture"), 3);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthTexture);

			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "ssaoTexture"), 4);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, ssaoTexture);

			glUniform1i(glGetUniformLocation(shader_quad->GetProgram(), "blurTexture"), 5);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, blurTexture);

			glUniformMatrix4fv(glGetUniformLocation(shader_quad->GetProgram(), "V"), 1, false, glm::value_ptr(camera->GetViewMat()));
			glUniformMatrix4fv(glGetUniformLocation(shader_quad->GetProgram(), "P"), 1, false, glm::value_ptr(camera->projection));

			glDrawElements(GL_TRIANGLES, screenQuad.count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
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
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			unsigned int width = camera->gameObject->scene->height;
			unsigned int height = camera->gameObject->scene->width;

			glGenTextures(1, &posTexture);
			glBindTexture(GL_TEXTURE_2D, posTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, height, width, 0, GL_RGB, GL_FLOAT, NULL);///////////////////////////////////////////////// HEIGHT WIDTH?!?!?
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTexture, 0);

			glGenTextures(1, &normTexture);
			glBindTexture(GL_TEXTURE_2D, normTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, height, width, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTexture, 0);

			glGenTextures(1, &colorTexture);
			glBindTexture(GL_TEXTURE_2D, colorTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, height, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTexture, 0);

			glGenTextures(1, &depthTexture);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, height, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

			GLuint att[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, att);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "Framebuffer incomplete!\n";
			}

			//ssao framebuffer
			glGenFramebuffers(1, &framebufferSsao);
			glBindFramebuffer(GL_FRAMEBUFFER, framebufferSsao);

			glGenTextures(1, &ssaoTexture);
			glBindTexture(GL_TEXTURE_2D, ssaoTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, height, width, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture, 0);

			GLuint att2[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, att2);


			//blur framebuffer
			glGenFramebuffers(1, &framebufferBlur);
			glBindFramebuffer(GL_FRAMEBUFFER, framebufferBlur);

			glGenTextures(1, &blurTexture);
			glBindTexture(GL_TEXTURE_2D, blurTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, height, width, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexture, 0);

			GLuint att3[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, att3);


			/*glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_HEIGHT, WINDOW_WIDTH);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);*/


			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "Framebuffer SSAO incomplete!\n";
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void GenKernelHemisphere(glm::vec3*& kernel) {
			kernel = new glm::vec3[KERNEL_SIZE];
			for (unsigned int i = 0; i < KERNEL_SIZE; i++) {
				float scale = (float)i / (float)(KERNEL_SIZE);
				glm::vec3 v;
				v.x = 2.0f * (float)rand() / RAND_MAX - 1.0f;
				v.y = 2.0f * (float)rand() / RAND_MAX - 1.0f;
				v.z = (float)rand() / RAND_MAX;

				v *= (0.1f + 0.9f * scale * scale);

				kernel[i] = v;
			}
			std::cout << "kern " << kernel[0].x;
		}

		void GenNoise(glm::vec3*& noise) {
			noise = new glm::vec3[NOISE_SIZE];
			for (unsigned int i = 0; i < NOISE_SIZE; i++) {
				float scale = (float)i / (float)(KERNEL_SIZE);
				glm::vec3 v;
				v.x = 2.0f * (float)rand() / RAND_MAX - 1.0f;
				v.y = 2.0f * (float)rand() / RAND_MAX - 1.0f;
				v.z = 0;

				noise[i] = v;
			}
		}

		void InitNoiseTexture() {
			glm::vec3* noise;
			GenNoise(noise);

			glGenTextures(1, &noiseTexture);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise[0]);
		}

		template <typename T>
		T* GetFirstComponentOfType() {
			for (auto& go : gameObjectVector) {
				T* component = go->GetComponent<T>();
				if (component != nullptr) {
					return component;
				}
			}
			return nullptr;
		}
	};
}