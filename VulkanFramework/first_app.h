#pragma once

#include "Window.h"
#include "EngineDevice.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SimpleRenderSystem.h"
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class FirstApp
{
public:
	static constexpr int width = 800;
	static constexpr int height = 600;

	FirstApp();
	~FirstApp();
	FirstApp(const FirstApp&) = delete;
	FirstApp& operator=(const FirstApp&) = delete;
	void run();
private:
	void loadGameObjects();
private:
	Window window{width, height, "Vulkan Framework"};
	EngineDevice device{window};
	Renderer renderer{window, device};
	std::vector<GameObject> gameObjects;
};