#include "dependences\glm\gtx\quaternion.hpp"
#include "dependences\glew\glew.h"
#include "dependences\glm\glm.hpp"
#include "dependences\glm\gtc\type_ptr.hpp"
#include "dependences\glm\gtc\matrix_transform.hpp"
#include "dependences\glm\gtc\quaternion.hpp"
#include "dependences\glew\glew.h"
#include <GL/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "dependences/imgui/imgui.h"
#include "dependences/imgui/imgui_impl_glfw_gl3.h"

#include "Input.hpp"
#include "Shader.hpp"
#include "Component.hpp"
#include "BaseTest.hpp"
#include "GameObject.hpp"

#include "Vertex.hpp"
#include "Util.hpp"
#include "RayCaster.hpp"
#include "Frustum.hpp"



#include "Transform.hpp"
#include "AxisAlignedBoundingBox.hpp"
#include "OrientedBoundingBox.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Debug.hpp"

#include "Material.hpp"
#include "MeshRenderer.hpp"
#include "AssetHelper.hpp"
#include "Object.hpp"
#include "LuaScript.hpp"
#include "Collider.hpp"
#include "BoxCollider.hpp"

#include "Collision.hpp"

#include "dependences/rapidxml/rapidxml.hpp"
#include "SceneReader.hpp"

#include "TestSimple.hpp"

#include <iostream>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	1000

static void ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glewInit();
	glfwSwapInterval(1);

	Test::TestSimple *test = new Test::TestSimple();
	Test::Init(test, window);

	ImGui_ImplGlfwGL3_Init(window, true);

	int height, width;
	glfwSetFramebufferSizeCallback(window, Test::ReshapeFunc);
	glfwGetFramebufferSize(window, &width, &height);
	test->ReshapeFunc(window, width, height);

	glfwSetKeyCallback(window, Test::KeyFunc);
	glfwSetCursorPosCallback(window, Test::CursorFunc);
	glfwSetMouseButtonCallback(window, Test::MouseButtonFunc);
	glfwSetScrollCallback(window, Test::ScrollFunc);

	unsigned int numFrames = 0;
	double time = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetTime() - time >= 1.0f) {
			time = glfwGetTime();
			numFrames = 0;
		}

		glfwPollEvents();
		test->Update();
		glfwSwapBuffers(window);

		numFrames++;
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

