#pragma once

#include "first_app.h"
#include <stdexcept>
#include <array>

FirstApp::FirstApp()
{
	loadGameObjects();
}

FirstApp::~FirstApp()
{}

void FirstApp::run()
{
	SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};

	while (!window.shouldClose())
	{
		glfwPollEvents();
		
		if (auto commandBuffer = renderer.beginFrame())
		{
			renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(device.device());
}

void FirstApp::loadGameObjects()
{
	std::vector<Model::Vertex> vertices {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	auto model = std::make_shared<Model>(device, vertices);

	auto triangle = GameObject::createGameObject();
	triangle.model = model;
	triangle.color = { 0.1f, 0.8f, 0.1f };
	triangle.transform2d.translation.x = 0.2f;
	triangle.transform2d.scale = { 0.5f, 0.2f };
	triangle.transform2d.rotation = 0.5f * glm::pi<float>();

	gameObjects.push_back(std::move(triangle));
}
