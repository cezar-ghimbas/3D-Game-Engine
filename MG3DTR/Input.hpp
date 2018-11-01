#pragma once
#include <unordered_map>

namespace Engine {
	struct Input {
		std::unordered_map<int, bool> keysState;

		Input() {}

		void ProcessKey(int key, int action) {
			if (action == GLFW_REPEAT || action == GLFW_PRESS) {
				keysState[key] = true;
			}
			else if (action == GLFW_RELEASE) {
				keysState[key] = false;
			}
		}

		bool GetKey(int key) {
			if (keysState.find(key) == keysState.end()) {
				return false;
			}
			return keysState[key];
		}
	};
}