#pragma once
#include <vector>

namespace Engine {

	struct GameObject {
	//private:
		std::vector<GameObject*> children;
		std::vector<Component*> components;

	public:
		GameObject* parent;
		Test::BaseTest* scene;
	
		GameObject() {
			parent = nullptr;
		}

		GameObject(Test::BaseTest* scene) : scene(scene) {
			parent = nullptr;
		}

		void Update() {
			for (auto& component : components) {
				if (component->enabled) {
					component->Update();
				}
			}
		}

		void UpdateAll() {
			for (auto& component : components) {
				component->Update();
			}

			for (auto& child : children) {
				child->Update();
			}
		}

		Transform* GetTransform() {
			assert(components.size() > 0 && "Error game object doesn't have transform attached");
			return (Transform*)components[0];
		}

		void AddChild(GameObject* child) {
			children.push_back(child);
			child->parent = this;
		}

		void AddComponent(Component* component) {
			components.push_back(component);
			component->SetGameObject(this);
			//component->enabled = true;
		}

		std::vector<GameObject*> GetChildren() {
			return children;
		}

		std::vector<GameObject*> GetAllGameObjects() {
			return scene->gameObjectVector;
		}

		template <typename T>
		T* GetComponent() {
			for (auto& component : components) {
				if (T* ret = dynamic_cast<T*>(component)) {
					return ret;
				}
			}
			return nullptr;
		}
	};
};