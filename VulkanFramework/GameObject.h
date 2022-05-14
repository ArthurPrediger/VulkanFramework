#pragma once

#include "Model.h"
#include <memory>

class GameObject
{
	struct Transform2dComponet
	{
		glm::vec2 translation{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation = 0.0f;

		glm::mat2 mat2() { 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c, s}, {-s, c} };

			glm::mat2 scaleMatrix{ {scale.x, 0.0f}, {0.0f, scale.y} };
			return rotMatrix * scaleMatrix;
		}
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
	Transform2dComponet transform2d;
private:
	GameObject(id_t objId)
		:
		id(objId)
	{}
private:
	id_t id;
};