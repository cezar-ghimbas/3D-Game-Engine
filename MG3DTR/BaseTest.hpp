#pragma once
#include "GLFWHelper.hpp"
#include <vector>

namespace Test {
	class BaseTest : public GLFWHelper {
	public:
		std::vector<Engine::GameObject*> gameObjectVector;
		Engine::Shader* shader;
		Engine::Input* input;
		unsigned int width;
		unsigned int height;
		BaseTest(){}
	};
}