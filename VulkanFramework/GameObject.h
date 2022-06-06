#pragma once

#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class GameObject
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

public:
	using id_t = unsigned char;

	static GameObject createGameObject()
	{
		static id_t currentId = 0;
		return GameObject(currentId++);
	}

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator=(GameObject&&) = default;

	id_t getId()
	{
		return id;
	}

public:
	std::shared_ptr<Model> model{};
	glm::vec3 color{};
	TransformComponent transform;
private:
	GameObject(id_t objId)
		:
		id(objId)
	{}
private:
	id_t id;
};