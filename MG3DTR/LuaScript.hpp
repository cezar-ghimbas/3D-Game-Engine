#pragma once

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <LuaBridge.h>
#include <sol.hpp>


namespace Engine {
	struct LuaScript : public Component {

		sol::state luaState;

		LuaScript(const std::string& scriptFile) {
			luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os);
			luaState.script_file(scriptFile);

			luaState.new_usertype<glm::vec3>("Vec3",
				sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
				"x", &glm::vec3::x,
				"y", &glm::vec3::y,
				"z", &glm::vec3::z);

			luaState.new_usertype<GameObject>("GameObject",
				sol::constructors<GameObject()>(),
				"GetTransform", &GameObject::GetTransform,
				"GetCamera", &GameObject::GetComponent<Camera>,
				"GetMeshRenderer", &GameObject::GetComponent<MeshRenderer>,
				"GetLight", &GameObject::GetComponent<Light>,
				"GetOrientedBoundingBox", &GameObject::GetComponent<OrientedBoundingBox>,
				"AxisAlignedBoundingBox", &GameObject::GetComponent<AxisAlignedBoundingBox>);

			luaState.new_usertype<Component>("Component",
				sol::constructors<Component()>(),
				"gameObject", &Component::gameObject,
				"enabled", &Component::enabled);

			luaState.new_usertype<Transform>("Transform",
				sol::constructors<Transform(), Transform(glm::vec3, glm::vec3, glm::vec3)>(),
				"position", &Transform::position,
				"scale", &Transform::scale,
				sol::base_classes, sol::bases<Component>());

			luaState.new_usertype<Camera>("Camera",
				sol::constructors<Camera(const glm::vec3&, const glm::vec3&, const glm::vec3&, float, float, float, float)>(),
				"TranslateForward", &Camera::TranslateForward,
				"TranslateRight", &Camera::TranslateRight,
				"TranslateUp", &Camera::TranslateUp,
				"SetFielfOfView", &Camera::SetFieldOfView,
				"SetNear", &Camera::SetNear,
				"SetFar", &Camera::SetFar,
				"SetAspectRatio", &Camera::SetAspectRatio,
				sol::base_classes, sol::bases<Component>());
				
				AddMaterialClass();
				AddMeshRendererClass();
				AddLightClass();
				AddOrientedBoundingBoxClass();
				AddInputClass();
				AddImGuiFunctions();
		}

		void AddMaterialClass() {
			luaState.new_usertype<Material>("Material",
				sol::constructors<Material(), Material(glm::vec3, glm::vec3, glm::vec3, float)>(),
				"ambient", &Material::ambient,
				"diffuse", &Material::diffuse,
				"specular", &Material::specular,
				"shininess", &Material::shininess);
		}

		void AddMeshRendererClass() {
			luaState.new_usertype<MeshRenderer>("MeshRenderer",
				"material", &MeshRenderer::material,
				sol::base_classes, sol::bases<Component>());
		}

		void AddLightClass() {
			luaState.new_usertype<Light>("Light",
				"color", &Light::color,
				sol::base_classes, sol::bases<Component>());
		}

		struct Vector {
			std::vector<glm::vec3> vec;

			Vector(const std::vector <glm::vec3>& v) : vec(v) {}

			glm::vec3 Get(int index) {
				return vec[index];
			}
		};

		struct VectorProxy {
			static Vector Get(OrientedBoundingBox* obb) {
				return Vector(obb->GetOBBPts());
			}
		};

		void AddOrientedBoundingBoxClass() {
			luaState.new_usertype<OrientedBoundingBox>("OrientedBoundingBox",
				"GetOBBPts", &OrientedBoundingBox::GetOBBPts,
				sol::base_classes, sol::bases<Component>());
		}

		void AddAxisAlignedBoundingBoxClass() {
			luaState.new_usertype<AxisAlignedBoundingBox>("AxisAlignedBoundingBox",
				"GetAABBPts", &AxisAlignedBoundingBox::GetAABBPts,
				sol::base_classes, sol::bases<Component>());
		}

		void TestFunc(int val) {
		}

		void AddImGuiFunctions() {
			sol::table guiTable = luaState.create_named_table("GUI");

			guiTable.set_function("NewFrame", &ImGui_ImplGlfwGL3_NewFrame);
			guiTable.set_function("Render", &ImGui::Render);
			guiTable.set_function("Begin", [](std::string windowName, bool open) { 
				ImGui::Begin(windowName.c_str(), &open); 
				return open; 
			});

			guiTable.set_function("End", &ImGui::End);
			guiTable.set_function("Button", [](const std::string& buttonName) { return ImGui::Button(buttonName.c_str()); });
			guiTable.set_function("Text", &ImGui::Text);
			guiTable.set_function("SliderInt", [](const std::string& label, int value, int minValue, int maxValue) {
				ImGui::SliderInt(label.c_str(), &value, minValue, maxValue);
				return value;
			});
			guiTable.set_function("Checkbox", [](const std::string& label, bool value) {
				ImGui::Checkbox(label.c_str(), &value);
				return value;
			});

			guiTable.set_function("RadioButton", [](const std::string& label, int value, int valueButton) {
				ImGui::RadioButton(label.c_str(), &value, valueButton);
				return value;
			});

			guiTable.set_function("InputText", [](const std::string& label, int size) {
				char* data = new char[size];
				ImGui::InputText(label.c_str(), data, size);

			});

			guiTable.set_function("InputInt", [](const std::string& label, int val) {
				ImGui::InputInt(label.c_str(), &val);
				return val;
			});


			guiTable.set_function("InputFloat", [](const std::string& label, float val) {
				ImGui::InputFloat(label.c_str(), &val);
				return val;
			});

			guiTable.set_function("ListBox", [](const std::string& label, int currentItem, sol::table stringArr) {
				char** itemList = new char*[stringArr.size()];
				int i = 0;
				for (auto el : stringArr) {
					itemList[i] = new char[el.second.as<std::string>().size() + 1];
					strcpy(itemList[i++], el.second.as<std::string>().c_str());
				}
				ImGui::ListBox(label.c_str(), &currentItem, itemList, stringArr.size());
				return currentItem;
			});

			guiTable.set_function("BeginMenu", [](const std::string& label) { return ImGui::BeginMenu(label.c_str()); });
			guiTable.set_function("MenuItem", [](const std::string& label, const std::string& shortcut) {
				return ImGui::MenuItem(label.c_str(), shortcut.c_str());
			});
			guiTable.set_function("EndMenu", &ImGui::EndMenu);

			guiTable.set_function("OpenPopup", [](const std::string& label) { return ImGui::OpenPopup(label.c_str()); });
			guiTable.set_function("BeginPopup", [](const std::string& label) { return ImGui::BeginPopup(label.c_str()); });
			guiTable.set_function("EndPopup", &ImGui::EndPopup);
			guiTable.set_function("Selectable", [](const std::string& label) { return ImGui::Selectable(label.c_str()); });

			///////////
			guiTable.set_function("InputInt2", [](const std::string& label, int val1, int val2) {
				int v[2] = { val1, val2 };
				ImGui::InputInt2(label.c_str(), v);
				return std::make_tuple(val1, val2);
			});

			guiTable.set_function("TestMultipleReturn", []() {
				return std::make_tuple(10, 3.4);
			});
		}

		void AddInputClass() {
			luaState.new_usertype<Input>("Input",
				sol::constructors<Input()>(),
				"GetKey", &Input::GetKey);

			luaState["KEY_W"] = GLFW_KEY_W;
			luaState["KEY_A"] = GLFW_KEY_A;
			luaState["KEY_S"] = GLFW_KEY_S;
			luaState["KEY_D"] = GLFW_KEY_D;
			luaState["KEY_0"] = GLFW_KEY_0;
			luaState["KEY_1"] = GLFW_KEY_1;
			luaState["KEY_2"] = GLFW_KEY_2;
			luaState["KEY_3"] = GLFW_KEY_3;
			luaState["KEY_4"] = GLFW_KEY_4;
			luaState["KEY_5"] = GLFW_KEY_5;
			luaState["KEY_6"] = GLFW_KEY_6;
			luaState["KEY_7"] = GLFW_KEY_7;
			luaState["KEY_8"] = GLFW_KEY_8;
			luaState["KEY_9"] = GLFW_KEY_9;
		}


		void SetGameObject(GameObject* gameObject) override {
			this->gameObject = gameObject;
			luaState["gameObject"] = gameObject;
			luaState["input"] = gameObject->scene->input;
			auto startFunc = luaState["start"];
			startFunc();
		}

		void Update() override {
			auto updateFunc = luaState["update"];
			updateFunc();
		}

		~LuaScript() {
			lua_close(luaState);
			delete luaState;
		}
	};
}