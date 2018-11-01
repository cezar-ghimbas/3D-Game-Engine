#pragma once

namespace Engine {

	Component* CreateTransform(rapidxml::xml_node<>* transformNode) {
		rapidxml::xml_node<>* posNode = transformNode->first_node("Position");
		rapidxml::xml_node<>* rotNode = transformNode->first_node("Rotation");
		rapidxml::xml_node<>* scaleNode = transformNode->first_node("Scale");
		glm::vec3 pos = glm::vec3(std::stof(posNode->first_attribute("x")->value()), std::stof(posNode->first_attribute("y")->value()), std::stof(posNode->first_attribute("z")->value()));
		glm::vec3 rot = glm::vec3(glm::radians(std::stof(rotNode->first_attribute("x")->value())), glm::radians(std::stof(rotNode->first_attribute("y")->value())), glm::radians(std::stof(rotNode->first_attribute("z")->value())));
		glm::vec3 scale = glm::vec3(std::stof(scaleNode->first_attribute("x")->value()), std::stof(scaleNode->first_attribute("y")->value()), std::stof(scaleNode->first_attribute("z")->value()));
		return new Transform(pos, rot, scale);
	}

	Component* CreateMeshRenderer(rapidxml::xml_node<>* meshRendererNode, Test::BaseTest* scene) {
		std::string vertFile = std::string(meshRendererNode->first_node("VertexFile")->first_attribute("location")->value());
		std::string textureFile = std::string(meshRendererNode->first_node("TextureFile")->first_attribute("location")->value());

		std::vector<Engine::Vertex> verts;
		Engine::Material material;
		Engine::ReadOBJ(vertFile, verts, material);

		unsigned int texture = Engine::LoadTexture(textureFile);
		unsigned int meshVao, meshCount;
		Engine::SendVertsToGPU(verts, meshVao, meshCount);
		return new Engine::MeshRenderer(meshVao, meshCount, scene->shader, texture, material);
	}

	Component* CreateOrientedBoundingBox(rapidxml::xml_node<>* orientedBoundingBoxNode) {
		std::string vertFile = std::string(orientedBoundingBoxNode->first_node("VertexFile")->first_attribute("location")->value());
		std::vector<Engine::Vertex> verts;
		Engine::Material material;
		Engine::ReadOBJ(vertFile, verts, material);

		glm::vec3 bottomLeft, topRight;
		Engine::AxisAlignedBoundingBox::GetAABBLimits(verts, bottomLeft, topRight);
		return new Engine::OrientedBoundingBox(bottomLeft, topRight);
	}

	Component* CreateCamera(rapidxml::xml_node<>* cameraNode, Engine::GameObject* gameObject, Test::BaseTest* scene) {
		float fov = std::stof(cameraNode->first_node("Fov")->first_attribute("value")->value());
		float near = std::stof(cameraNode->first_node("Near")->first_attribute("value")->value());
		float far = std::stof(cameraNode->first_node("Far")->first_attribute("value")->value());
		float xLookAt = std::stof(cameraNode->first_node("LookAt")->first_attribute("x")->value());
		float yLookAt = std::stof(cameraNode->first_node("LookAt")->first_attribute("y")->value());
		float zLookAt = std::stof(cameraNode->first_node("LookAt")->first_attribute("z")->value());
		float xUp = std::stof(cameraNode->first_node("Up")->first_attribute("x")->value());
		float yUp = std::stof(cameraNode->first_node("Up")->first_attribute("y")->value());
		float zUp = std::stof(cameraNode->first_node("Up")->first_attribute("z")->value());


		return new Engine::FirstPersonCamera(gameObject->GetTransform()->position,
			glm::vec3(xLookAt, yLookAt, zLookAt), 
			glm::vec3(xUp, yUp, zUp), fov, 
			(float)scene->width / (float)scene->height, near, far);
	}

	Component* CreateLight(rapidxml::xml_node<>* lightNode) {
		float redValue = std::stof(lightNode->first_node("Color")->first_attribute("x")->value());
		float greenValue = std::stof(lightNode->first_node("Color")->first_attribute("y")->value());
		float blueValue = std::stof(lightNode->first_node("Color")->first_attribute("z")->value());

		return new Light(glm::vec3(redValue, greenValue, blueValue));
	}

	Component* CreateScript(rapidxml::xml_node<>* scriptNode) {
		std::string scriptFile = std::string(scriptNode->first_node("ScriptFile")->first_attribute("location")->value());

		return new LuaScript(scriptFile);
	}

	Component* CreateBoxCollider(rapidxml::xml_node<>* colliderNode, Test::BaseTest* scene) {
		return new BoxCollider();
	}


	GameObject* CreateGameObject(rapidxml::xml_node<>* objectNode, Test::BaseTest* scene) {
		GameObject* ret = new GameObject(scene);
		rapidxml::xml_node<>* currentNode;

		currentNode = objectNode->first_node("Transform");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateTransform(currentNode));
		}

		currentNode = objectNode->first_node("MeshRenderer");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateMeshRenderer(currentNode, scene));
		}

		currentNode = objectNode->first_node("OrientedBoundingBox");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateOrientedBoundingBox(currentNode));
		}

		currentNode = objectNode->first_node("Camera");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateCamera(currentNode, ret, scene));
		}

		currentNode = objectNode->first_node("Light");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateLight(currentNode));
		}

		currentNode = objectNode->first_node("Script");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateScript(currentNode));
		}

		currentNode = objectNode->first_node("BoxCollider");
		if (currentNode != nullptr) {
			ret->AddComponent(CreateBoxCollider(currentNode, scene));
		}

		currentNode = objectNode->first_node("Object");
		if (currentNode != nullptr) {
			GameObject* childGameObject = CreateGameObject(currentNode, scene);
			ret->AddChild(childGameObject);
		}

		return ret;
	}

	void AddGameObjectTree(GameObject* gameObject, std::vector<GameObject*>& goVector) {
		goVector.push_back(gameObject);
		for (auto& go : gameObject->GetChildren()) {
			AddGameObjectTree(go, goVector);
		}
	}

	std::vector<GameObject*> CreateScene(const std::string& sceneFile, Test::BaseTest* scene) {
		std::vector<GameObject*> gameObjectVector;
		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* root;
		std::ifstream inFile(sceneFile);
		std::vector<char> buf((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		buf.push_back('\0');
		doc.parse<0>(&buf[0]);
		root = doc.first_node("Scene");

		for (rapidxml::xml_node<>* objectNode = root->first_node("Object"); objectNode; objectNode = objectNode->next_sibling()) {
			AddGameObjectTree(CreateGameObject(objectNode, scene),gameObjectVector);
		}
		return gameObjectVector;
	}
}