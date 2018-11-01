#pragma once

namespace Engine {

	struct GameObject;
	struct Transform;

	struct Component {

		GameObject* gameObject;
		bool enabled;

		Component() : enabled(true) {}

		virtual void SetGameObject(GameObject* gameObject) {
			this->gameObject = gameObject;
		}

		virtual void Update() {}

		virtual ~Component() {}

	};

}